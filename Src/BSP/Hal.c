/*****************************************************************************
 @Project	: 
 @File 		: Hal.c
 @Details  	: All Ports and peripherals configuration                    
 @Author	: 
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/
#include <Common.h>
#include "Hal.h"


/*****************************************************************************
 Define
******************************************************************************/
#define BT_ENABLE
#define IMU_ENABLE


/*****************************************************************************
 Type definition
******************************************************************************/


/*****************************************************************************
 Global Variables
******************************************************************************/


/*****************************************************************************
 Local Variables
******************************************************************************/


/*****************************************************************************
 Implementation
******************************************************************************/
void PortInit( void )
{
    int32_t tmp = 0;

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN 
                                    | RCC_APB2ENR_IOPBEN
                                    | RCC_APB2ENR_TIM1EN
                                    | RCC_APB2ENR_IOPCEN
                                    | RCC_APB2ENR_IOPDEN
                                    | RCC_APB2ENR_AFIOEN;

    GPIOA->CRL &= ~GPIO_CRL_CNF5;
    GPIOA->CRL |= GPIO_CRL_MODE5_0 | GPIO_CRL_MODE5_1; /* LED PA5 */



    /************************************************************************
       Initialize I2C 2
          SCL = PB10
          SDA = PB11
    *************************************************************************/	
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    GPIOB->CRH |= GPIO_CRH_MODE10_0 | GPIO_CRH_MODE10_1;    /* PB10 I2C SCL */
    GPIOB->CRH |= GPIO_CRH_CNF10_0;
    GPIOB->CRH |= GPIO_CRH_CNF10_1;
    
    GPIOB->CRH |= GPIO_CRH_MODE11_0 | GPIO_CRH_MODE11_1;    /* PB11 I2C SCK */
    GPIOB->CRH |= GPIO_CRH_CNF11_0;
    GPIOB->CRH |= GPIO_CRH_CNF11_1;

}


























