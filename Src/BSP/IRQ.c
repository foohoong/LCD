/*****************************************************************************
 @Project	: 
 @File 		: BSP.c
 @Details  	:
 @Author	: 
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/
#include <Common.h>
#include "BSP.h"
#include "IRQ.h"


/*****************************************************************************
 Define
******************************************************************************/


/*****************************************************************************
 Type definition
******************************************************************************/


/*****************************************************************************
 Global Variables
******************************************************************************/
extern void EXTI8_M1_ENC_IN1_IRQHandler( uint32_t Status ) __attribute__((weak));
extern void EXTI9_M2_ENC_IN1_IRQHandler( uint32_t Status ) __attribute__((weak));
extern void EXTI10_M3_ENC_IN1_IRQHandler( uint32_t Status ) __attribute__((weak));
extern void EXTI11_M4_ENC_IN1_IRQHandler( uint32_t Status ) __attribute__((weak));



/*****************************************************************************
 Local Variables
******************************************************************************/
void IRQInit( void )
{
	NVIC_EnableIRQ( EXTI9_5_IRQn );
	NVIC_EnableIRQ( EXTI15_10_IRQn ); 
}


/*****************************************************************************
 Implementation
******************************************************************************/
void EXTI0_IRQHandler( void )
{
	uint32_t irqStatus = EXTI->PR;
}


void EXTI1_IRQHandler( void )
{
	uint32_t irqStatus = EXTI->PR;
}


void EXTI2_IRQHandler( void )
{
	uint32_t irqStatus = EXTI->PR;
}


void EXTI3_IRQHandler( void )
{
	uint32_t irqStatus = EXTI->PR;
}


void EXTI4_IRQHandler( void )
{
	uint32_t irqStatus = EXTI->PR;
}


void EXTI9_5_IRQHandler( void )
{
	uint32_t irqStatus = EXTI->PR;
	
	if( 0 != (irqStatus & EXTI_PR_PR8) )
	{
		EXTI->PR = EXTI_PR_PR8;
		EXTI8_M1_ENC_IN1_IRQHandler( irqStatus );
	}

	if( 0 != (irqStatus & EXTI_PR_PR9) )
	{
		EXTI->PR = EXTI_PR_PR9;
		EXTI9_M2_ENC_IN1_IRQHandler( irqStatus );
	}
}


void EXTI15_10_IRQHandler( void )
{
	uint32_t irqStatus = EXTI->PR;
	
	if( 0 != (irqStatus & EXTI_PR_PR10) )
	{
		EXTI->PR = EXTI_PR_PR10;
		EXTI10_M3_ENC_IN1_IRQHandler( irqStatus );
	}

	if( 0 != (irqStatus & EXTI_PR_PR11) )
	{
		EXTI->PR = EXTI_PR_PR11;
		EXTI11_M4_ENC_IN1_IRQHandler( irqStatus );
	}
}



/*****************************************************************************
 Callback functions
******************************************************************************/


/*****************************************************************************
 Local functions
******************************************************************************/


/*****************************************************************************
 Interrupt functions
******************************************************************************/






















