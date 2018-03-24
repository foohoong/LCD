/*****************************************************************************
 @Project	: 
 @File 		: rtc.c
 @Details  	: STM32F103XX rtc driver               
 @Author	: 
 @Hardware	: STM32
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  lcgan     XXXX-XX-X X  		Initial Release
******************************************************************************/
#include <Common.h>
#include "Hal.h"
#include "dma.h"


/*****************************************************************************
 Define
******************************************************************************/
#define TOTAL_DMA		2U
#define TOTAL_STREAM	7U


/*****************************************************************************
 Type definition
******************************************************************************/

#define DMA1_Stream0	DMA1_BASE
#define DMA1_Stream1	DMA1_Channel1_BASE
#define DMA1_Stream2	DMA1_Channel2_BASE
#define DMA1_Stream3	DMA1_Channel3_BASE
#define DMA1_Stream4	DMA1_Channel4_BASE
#define DMA1_Stream5	DMA1_Channel5_BASE
#define DMA1_Stream6	DMA1_Channel6_BASE
#define DMA1_Stream7	DMA1_Channel7_BASE


#define DMA2_Stream0	DMA2_BASE
#define DMA2_Stream1	DMA2_Channel1_BASE
#define DMA2_Stream2	DMA2_Channel2_BASE
#define DMA2_Stream3	DMA2_Channel3_BASE
#define DMA2_Stream4	DMA2_Channel4_BASE
#define DMA2_Stream5	DMA2_Channel5_BASE


/*****************************************************************************
 Global Variables
******************************************************************************/

/*****************************************************************************
 Constant Variables
******************************************************************************/
static const DMA_Channel_TypeDef *DMA1_STREAM_TABLE[8] = 
{
	DMA1_Stream0,
	DMA1_Stream1,
	DMA1_Stream2,
	DMA1_Stream3,
	DMA1_Stream4,
	DMA1_Stream5,
	DMA1_Stream6,
	DMA1_Stream7
};


static const DMA_Channel_TypeDef *DMA2_STREAM_TABLE[8] = 
{
	DMA2_Stream0,
	DMA2_Stream1,
	DMA2_Stream2,
	DMA2_Stream3,
	DMA2_Stream4,
	DMA2_Stream5
};


/*****************************************************************************
 Local Variables
******************************************************************************/


/*****************************************************************************
 Local Functions
******************************************************************************/


/*****************************************************************************
 Implementation
******************************************************************************/
void DmaInit( int nPort, uint8_t nStream, DMA_CONFIG Direction, DMA_CONFIG MemIncrement, DMA_CONFIG PeriIncrement, int nChannel )
{
	DMA_TypeDef *dma;
    DMA_Channel_TypeDef *stream;
	uint32_t tmp;

	ASSERT( 0 != nPort );
	ASSERT( nPort<=TOTAL_DMA );
    ASSERT( nStream<=TOTAL_STREAM );

	switch( nPort )
	{
		case 1:
			dma = DMA1;
            stream = DMA2_STREAM_TABLE[nStream];
		break;

		case 2:
			dma = DMA2;
            stream = DMA2_STREAM_TABLE[nStream];
		break;

		default:
		break;
	}

	/* Disable DMA peripherals before configuration */
	stream->CR &= ~DMA_SxCR_EN;
	while( 0 != (stream->CR & DMA_SxCR_EN) );

	tmp = stream->CR;
	tmp &= ~( DMA_SxCR_CHSEL 
			| DMA_SxCR_MBURST 
			| DMA_SxCR_PBURST 
			| DMA_SxCR_PL
			| DMA_SxCR_MSIZE
			| DMA_SxCR_PSIZE
			| DMA_SxCR_MINC  
			| DMA_SxCR_PINC   
			| DMA_SxCR_CIRC   
			| DMA_SxCR_DIR   
			| DMA_SxCR_CT     
			| DMA_SxCR_DBM );

	switch( Direction )
	{
		case DMA_DIR_MEM_TO_PERI:
			tmp |= DMA_SxCR_DIR_0;
		break;

		case DMA_DIR_PERI_TO_MEM:
			tmp &= ~(DMA_SxCR_DIR_0 | DMA_SxCR_DIR_1);
		break;

		case DMA_DIR_MEM_TO_MEM:
			tmp |= DMA_SxCR_DIR_1;
		break;

		default:
		break;
	}

	switch( MemIncrement )
	{
		case DMA_MEM_INC_EN:
		break;

		case DMA_MEM_INC_DIS:
		break;

		default:
		break;
	}

	switch( MemIncrement )
	{
		case DMA_MEM_INC_EN:
		break;

		case DMA_MEM_INC_DIS:
		break;

		default:
		break;
	}

	switch( PeriIncrement )
	{
		case DMA_PERI_INC_EN:
		break;

		case DMA_PERI_INC_DIS:
		break;

		default:
		break;
	}
}

/*****************************************************************************
 Interrupt handling
******************************************************************************/




























