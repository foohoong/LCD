/******************************************************************************
 @filename		Serial.c

 @project			
 
 @description		
 
 @version		1.0.0

 @revision		
******************************************************************************/

#include <Common.h>
#include "Serial.h"


/******************************************************************************
 Define
******************************************************************************/


/******************************************************************************
 Global variables
******************************************************************************/


/******************************************************************************
 Typedef
******************************************************************************/


/******************************************************************************
 Local variables
******************************************************************************/


/*****************************************************************************
 Local function
*****************************************************************************/
static int	buf_Pull( PUART_FIFO pFifo, char *pData );
static int	buf_Push( PUART_FIFO pFifo, char data );


/*****************************************************************************
 Implementations
*****************************************************************************/
int
SerialInit(
	PUART_HANDLE		pHandle,
	UART_DRIVER const 	*pDriver,
	uint8_t 			nPort,
	uint32_t 			nBaud,
	char 				*pTxBuf,
	uint32_t 			nTxSize,
	char 				*pRxBuf,
	uint32_t 			nRxSize
	)
{
	int res = UART_STS_NO_PORT;
	
	ASSERT( 0 != pHandle );
	
	memset( pHandle, 0, sizeof( UART_HANDLE ) );
		
	pHandle->pDriver 		= pDriver;
	pHandle->TxFifo.nSize 	= nTxSize;
	pHandle->TxFifo.pBuf 	= pTxBuf;
	pHandle->RxFifo.nSize 	= nRxSize;
	pHandle->RxFifo.pBuf 	= pRxBuf;
	res = pDriver->pfInit( pHandle, nPort, nBaud );
	
	return res;
}


void 
SerialAddCallback(
	PUART_HANDLE	pHandle,
	UART_CB_ONTX	*pfOnTx,
	UART_CB_ONRX	*pfOnRx
	)
{
	ASSERT( 0 != pHandle );
	
	if( 0 != pfOnTx )
	{
		pHandle->pfOnTx = pfOnTx;
	}

	if( 0 != pfOnRx )
	{
		pHandle->pfOnRx = pfOnRx;
	}
}


int	
SerialBaudrate(
	PUART_HANDLE	pHandle,
	uint32_t		nBaud
	)
{
	ASSERT( 0 != pHandle );
	
	return pHandle->pDriver->pfBaudrate( pHandle, nBaud );
}


int
SerialConfig(
	UART_HANDLE const	*pHandle, 
	UART_CFG 			Databit, 
	UART_CFG 			Parity,
	UART_CFG 			Stopbit
	)
{
	ASSERT( 0 != pHandle );
	
	return pHandle->pDriver->pfConfig(
								pHandle,
								Databit,
								Parity,
								Stopbit );
}


int
SerialWrite(
	PUART_HANDLE 	pHandle,
	char 			Data
	)
{
	int res = 0;
	char tmp;
	
	ASSERT( 0 != pHandle );
	
	/* lock interrupt to prevent data contention */
	pHandle->pDriver->pfLock( pHandle );
	
	/* Check UART is ready to transmit */
	if( 0 == pHandle->pDriver->pfIsTxReady( pHandle ) )
	{
		/* Hardware buffer full, we push into software ring buffer */
		res = buf_Push( &pHandle->TxFifo, Data );
		
		/* Unlock the interrupt, lets it run */
		pHandle->pDriver->pfUnlock( pHandle );

		if( 0 == res )
		{
			/* s/w FIFO full */
			return UART_STS_BUF_OVER;
		}
	}
	else
	{
		if( pHandle->TxFifo.nHead != pHandle->TxFifo.nTail )
		{
			/* Hardware buffer full, we push into software ring buffer */
			res = buf_Push( &pHandle->TxFifo, Data );
			
			if( 1 == buf_Pull( &pHandle->TxFifo, &tmp ) )
			{
				/* Unlock the interrupt, lets it run */
				pHandle->pDriver->pfUnlock( pHandle );
				
				/* UART is empty, ready to write the data */
				pHandle->pDriver->pfWrite( pHandle, tmp );
			}
			else
			{		
				/* Unlock the interrupt, lets it run */
				pHandle->pDriver->pfUnlock( pHandle );
			}
		}
		else
		{
			/* Unlock the interrupt, lets it run */
			pHandle->pDriver->pfUnlock( pHandle );
			
			/* UART is empty, ready to write the data */
			pHandle->pDriver->pfWrite( pHandle, Data );
		}
	}
	
	return UART_STS_OK;
}


int
SerialWriteEx(
	PUART_HANDLE 	pHandle,
	void const		*pData,
    int             nSize
	)
{
    int cnt;
	char const *data = (char const *)pData;
	
	ASSERT( 0 != pHandle );
	
    for( cnt=0; cnt<nSize; cnt++ )
    {
        while( 0 == pHandle->pDriver->pfIsTxReady( pHandle ) );
    
         /* UART is empty, ready to write the data */
        pHandle->pDriver->pfWrite( pHandle, *data++ );
    }
	
	return UART_STS_OK;
}


char SerialRead( PUART_HANDLE pHandle )
{
	char 	data = 0;
	int 	res	 = 0;
	
	ASSERT( 0 != pHandle );
	
	/* atomic the proccess */
	pHandle->pDriver->pfLock( pHandle );
	res = buf_Pull( &pHandle->RxFifo, &data );
	pHandle->pDriver->pfUnlock( pHandle );
	
	/* ring buffer is empty will return 0 */
	return ((0 == res)? 0 : data);
}


void SerialTxEmpty( UART_HANDLE *pHandle )
{
	ASSERT( 0 != pHandle );
	
	pHandle->pDriver->pfLock( pHandle );
	pHandle->TxFifo.nHead = 0;
	pHandle->TxFifo.nTail = 0;
	pHandle->pDriver->pfUnlock( pHandle );
}


void SerialRxEmpty( UART_HANDLE *pHandle )
{
	ASSERT( 0 != pHandle );
	
	pHandle->pDriver->pfLock( pHandle );
	pHandle->RxFifo.nHead = 0;
	pHandle->RxFifo.nTail = 0;
	pHandle->pDriver->pfUnlock( pHandle );
}


BOOL SerialIsTxEmpty( UART_HANDLE const *pHandle )
{
	BOOL res;
	ASSERT( 0 != pHandle );
	
	pHandle->pDriver->pfLock( pHandle );
	res = pHandle->pDriver->pfIsTxEnd( pHandle );
	pHandle->pDriver->pfUnlock( pHandle );
	return res;
}


BOOL SerialIsTxReady( UART_HANDLE const *pHandle )
{
	int	cnt 	= 0;
	int tail 	= 0;
	
	ASSERT( 0 != pHandle );
	
	pHandle->pDriver->pfLock( pHandle );
	cnt = pHandle->TxFifo.nHead + 1U;
	if( cnt >= pHandle->TxFifo.nSize )
	{
		/* point back to the top of the FIFO ring buffer */
		cnt = 0;
	}
	tail = pHandle->TxFifo.nTail;
	pHandle->pDriver->pfUnlock( pHandle );
	
	/* FIFO full will return 0 */
	return ((cnt == tail)? FALSE : TRUE);
}


BOOL SerialIsRxReady( UART_HANDLE const *pHandle )
{
	ASSERT( 0 != pHandle );
	
	return ((pHandle->RxFifo.nHead != pHandle->RxFifo.nTail)? TRUE : FALSE);
}


/*****************************************************************************
 IRQ handler
*****************************************************************************/
void UartIsrTx( PUART_HANDLE pHandle )
{
	char data;
	
	ASSERT( 0 != pHandle );
	
	if( 1 == buf_Pull( &pHandle->TxFifo, &data ) )
	{
		/* Send it now */
		pHandle->pDriver->pfWrite( pHandle, data );
	}
	else
	{
		/* Nothing else to Tx, disable IRQ */
		pHandle->pDriver->pfTxEnd( pHandle );

		if( 0 != pHandle->pfOnTx )
		{
			pHandle->pfOnTx();
		}
	}
}


void UartIsrRx( PUART_HANDLE pHandle )
{
	char data;
    BOOL callback = FALSE;
	
	ASSERT( 0 != pHandle );
	
	data = pHandle->pDriver->pfRead( pHandle );

    if( pHandle->RxFifo.nHead == pHandle->RxFifo.nTail )
    {
        callback = TRUE;
    }

	if( 0 == buf_Push( &pHandle->RxFifo, data ) )
	{
		pHandle->nError |= UART_ERR_OVERFLOW;
	}
	else
	{
        if( TRUE == callback )
        {
            if( 0 != pHandle->pfOnRx )
            {
                pHandle->pfOnRx();
            }
        }
	}
}


/*****************************************************************************
 Local helpers
*****************************************************************************/

/* FIFO access */
static int buf_Push( PUART_FIFO	pFifo, char Data )
{
	int	hd;

	ASSERT( 0 != pFifo );
	
	/* Write TxHead to nTemp */
	hd = pFifo->nHead + 1;
	
	if( hd >= pFifo->nSize )
	{
		/* point back to the top of the FIFO ring buffer */
		hd = 0;
	}
	
	if( hd == pFifo->nTail )
	{
		return 0;						/* FIFO full */
	}
		
	pFifo->pBuf[pFifo->nHead] = Data;	/* copy data into FIFO */
	
	pFifo->nHead = hd;					/* update head pointer */

	return 1;
}


static int
buf_Pull(
	PUART_FIFO	pFifo,
	char	 	*pData
	)
{
	ASSERT( 0 != pFifo );
	ASSERT( 0 != pData );
	
	if( pFifo->nHead == pFifo->nTail )
	{
		return 0;							/* FIFO empty */
	}

	*pData = pFifo->pBuf[pFifo->nTail++];	/* get data from FIFO buffer */
	
	/* Check for buffer roll over */
	if( pFifo->nTail == pFifo->nSize )
	{
		pFifo->nTail = 0;		/* point back to the top of FIFO ring buffer */
	}
		
	return 1;
}


















