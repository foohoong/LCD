/*****************************************************************************
 @Project	: 
 @File 		: i2c.c
 @Details  	: 
 @Author		: lcgan
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/

#include <Common.h>
#include "Hal.h"
#include "i2c.h"


/*****************************************************************************
 Define
******************************************************************************/
#define TOTAL_I2C			2U


/*****************************************************************************
 Type definition
******************************************************************************/


/*****************************************************************************
 Global Variables
******************************************************************************/


/*****************************************************************************
 constant Variables
******************************************************************************/


/*****************************************************************************
 Local Variables
******************************************************************************/

static volatile PI2C_HANDLE	g_pI2cIrqHandles[TOTAL_I2C];


/*****************************************************************************
 Local functions
******************************************************************************/


/*****************************************************************************
 Callback functions
******************************************************************************/


/*****************************************************************************
 Implementation 
******************************************************************************/
int
I2cInit(
	PI2C_HANDLE 	pHandle,
	uint8_t 	nPort,
	uint32_t 	nBusSpeed
	)
{
	I2C_TypeDef 	*i2c;
	IRQn_Type 		irq;
	
	ASSERT( 0 != pHandle );
	
	switch( nPort )
	{
		case 0:
			i2c = I2C1;
			irq = I2C1_EV_IRQn;
		break;
		
		case 1:
			i2c = I2C2;
			irq = I2C2_EV_IRQn;
		break;
		
		default:
			return I2C_STS_PORT_INVALID;
	}

		
	/* Store callback function for ADC interrupt use */
	pHandle->pI2c = i2c;
	pHandle->bRepeatStart = FALSE;
	pHandle->nTxCount = 0;
	pHandle->nRxCount = 0;
	
	g_pI2cIrqHandles[nPort] = pHandle;
	
	i2c->CR1 = 0;
	
	i2c->CR1 &= ~I2C_CR1_PE;

	i2c->CR2 &= ~I2C_CR2_FREQ;
	i2c->CR2 |= 36;
	i2c->CCR |= I2C_CCR_FS | I2C_CCR_DUTY | 0x04;
	//i2c->TRISE = 0x09;
	i2c->CR1 |= I2C_CR1_PE;
	
	NVIC_EnableIRQ( irq );
	
	return I2C_STS_OK;
}


void
I2cWrite(
	PI2C_HANDLE pHandle,
	uint8_t 	nSlaveAddr,
	uint8_t 	Register,
	void const 	*pData,
	uint8_t 	nByte
	)
{
	I2C_TypeDef *i2c;
	ASSERT( 0 != pHandle );
	
	i2c = (I2C_TypeDef *)pHandle->pI2c;
	
	pHandle->nSlaveAddr	= nSlaveAddr;
	pHandle->Register	= Register;
	pHandle->pData		= (uint8_t *)pData;
	pHandle->nByte		= nByte;
	
	/* Start */
	i2c->OAR1 |= nSlaveAddr<<1;
	i2c->CR1 |= I2C_CR1_START;   /* generate Start  */
	i2c->CR2 |= I2C_CR2_ITEVTEN;  /* Event interrupt enable  */
}



void
I2cRead( 
	PI2C_HANDLE pHandle,
	uint8_t 	nSlaveAddr,
	uint8_t 	Register,
	void 		*pData,
	uint8_t 	nByte
	)
{
	I2C_TypeDef *i2c;
	ASSERT( 0 != pHandle );
	
	i2c = (I2C_TypeDef *)pHandle->pI2c;
	pHandle->nSlaveAddr	= nSlaveAddr | 0x01;
	pHandle->Register	= Register;
	pHandle->pData		= (uint8_t *)pData;
	pHandle->nByte		= nByte;
	
	/* Start */
	i2c->CR1 |= I2C_CR1_ACK | I2C_CR1_START; /* Ack enable, generate Start  */
	i2c->CR2 |= I2C_CR2_ITEVTEN;	/* Event interrupt enable  */
}


void 
I2cAddHook(
	PI2C_HANDLE 		pHandle,
	PI2C_HOOK 		pHook,
	I2C_CB_DONE 		*pfDone
	)
{
	ASSERT( 0 != pHandle );
	ASSERT( 0 != pHook );
	ASSERT( 0 != pfDone );
	
	pHook->pfDone 		= pfDone;
	pHook->pNext 		= pHandle->pHeadHook;
	pHandle->pHeadHook 	= pHook;
}


/*****************************************************************************
 Callback functions
******************************************************************************/


/*****************************************************************************
 Local functions
******************************************************************************/
static void i2c_Handler( PI2C_HANDLE pHandle, I2C_TypeDef *pI2C )
{
	PI2C_HOOK cur;
	uint32_t status = pI2C->SR1;
	uint32_t err = pI2C->SR2;

	
	ASSERT( 0 != pHandle );

	if( 0 != (status & I2C_SR1_SB) ) /* start bit  */
	{
		if( FALSE != pHandle->bRepeatStart )  /* Repeat Start */
		{
			pHandle->bRepeatStart = FALSE;
			
			pI2C->DR = pHandle->nSlaveAddr;	/* Read */
		}
		else  /* Start condition generated  */
		{
			pI2C->DR = pHandle->nSlaveAddr & 0xfe; /* Write- 7 bit addr mode*/
		}

		pHandle->nTxCount = 0;
		pHandle->nRxCount = 0;
		
		return;
	}
	
	if( 0 != (status & I2C_SR1_TXE) ) /* Tx data empty */
	{
		if( 0 != (status & I2C_SR1_ADDR) )  /* addr sent (master mode) & matched with slave addr  */
		{
			pI2C->DR = pHandle->Register;
		}
		else
		{
			if( 0 != (pHandle->nSlaveAddr&0x01) )
			{
				if( 0 == pHandle->nRxCount )
				{
					pHandle->bRepeatStart = TRUE;
					pI2C->CR1 |= I2C_CR1_START; /* Start generation */
				}
			}
			else
			{
				pI2C->DR = *pHandle->pData++;
				pHandle->nTxCount++;
				
				if( pHandle->nTxCount >= pHandle->nByte )
				{
					pI2C->CR1 |= I2C_CR1_STOP;  /* Stop generation */
					pI2C->CR2 &= ~I2C_CR2_ITEVTEN; /* Event Intr enable  */
				
					/* Callback to application to notify I2C is done */
					if( 0 != pHandle->pHeadHook )
					{
						/* Iterate a link list */
						for( cur=pHandle->pHeadHook; 0 != cur; cur=cur->pNext )
						{
							ASSERT( 0 != cur->pfDone );
							cur->pfDone();
						}
					}
				}
			}
		}
		
		return;
	}
	

	if( 0 != (status & I2C_SR1_ADDR) )  /* addr sent (master mode)   */
	{
		if( 1 == pHandle->nByte )
		{
			pI2C->CR1 &= ~I2C_CR1_ACK;  /* acknowledgment - for Rx  */
			return;
		}
	}
	
	if( 0 != (status & I2C_SR1_RXNE) )  /* receiver DR not empty */
	{
		if( 1 == pHandle->nByte )
		{
			pI2C->CR1 |= I2C_CR1_STOP;
		}
		
		*pHandle->pData++ = pI2C->DR;
		pHandle->nRxCount++;
		
		if( pHandle->nRxCount == (pHandle->nByte - 1) )
		{
			*pHandle->pData++ = pI2C->DR;
			pHandle->nRxCount++;
		}
		
		if( pHandle->nRxCount >= pHandle->nByte )
		{
			/* All bytes received done */
			pI2C->CR2 &= ~I2C_CR2_ITEVTEN;
			
			/* Callback to application to notify I2C is done */
			if( 0 != pHandle->pHeadHook )
			{
				/* Iterate a link list */
				for( cur=pHandle->pHeadHook; 0 != cur; cur=cur->pNext )
				{
					ASSERT( 0 != cur->pfDone );
					cur->pfDone();
				}
			}
			
			pHandle->nRxCount = 0;
		}
		
		if( pHandle->nByte > 2 )
		{
			if( pHandle->nRxCount == (pHandle->nByte - 2) )
			{
				/* Generate NACK for last 2nd byte */
				pI2C->CR1 &= ~I2C_CR1_ACK;
				pI2C->CR1 |= I2C_CR1_STOP;
			}
		}
	}
}


/*****************************************************************************
 Interrupt functions
******************************************************************************/
void I2C1_EV_IRQHandler( void )
{
	i2c_Handler( g_pI2cIrqHandles[0], I2C1 );
}


void I2C2_EV_IRQHandler( void )
{
	i2c_Handler( g_pI2cIrqHandles[1], I2C2 );	
}














