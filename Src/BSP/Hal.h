/*****************************************************************************
 @Project	: 
 @File 		: 
 @Details  	: All Ports and peripherals configuration                    
 @Author	: 
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/

#ifndef __HAL_DOT_H__
#define __HAL_DOT_H__


/*****************************************************************************
 Define
******************************************************************************/
#define PA_LED						5U

#define PA_M1_DIR					1U
#define PA_M2_DIR					4U
#define PB_M3_DIR					4U
#define PB_M4_DIR					5U

#define PA_M1_ENC_INA				8U
#define PC_M2_ENC_INA				2U
#define PC_M3_ENC_INA				3U
#define PA_M4_ENC_INA				11U
#define PC_M1_ENC_INB				8U
#define PC_M2_ENC_INB				9U
#define PC_M3_ENC_INB				10U
#define PC_M4_ENC_INB				11U

#define PD_US0_TRIG					2U
#define PA_US1_TRIG					7U
#define PB_US2_TRIG					0U
#define PB_US3_TRIG					1U
#define PA_US4_TRIG					12U
#define PC_US5_TRIG					1U
#define PA_US6_TRIG					PA_LED
#define PD_US7_TRIG					1U

#define PC_BTN_ECHO_US0				13U
#define PC_BTN_ECHO_US1				12U
#define PC_BTN_ECHO_US2				6U
#define PC_BTN_ECHO_US3				7U
#define PC_BTN_ECHO_US4 			4U
#define PC_BTN_ECHO_US5 			5U
#define PA_BTN_ECHO_US6 			15U
#define PB_BTN_ECHO_US7 			2U

#define PC_TAC_BTN2					4U
#define PC_REMOTE_ELEV_HORIZON		2U
#define PC_REMOTE_ELEV_VERTICAL		3U

#define PC_UCAM_RESET				0U


/*****************************************************************************
 Type definition
******************************************************************************/

/*****************************************************************************
 Macro
******************************************************************************/
#define LED_ON()                (GPIOA->BSRR = BIT(PA_LED))
#define LED_OFF()               (GPIOA->BRR = BIT(PA_LED))
#define LED_SET( x )            ((x)? LED_ON() : LED_OFF())

#define TEST_ON()               (GPIOC->BSRR = BIT(PC_TEST))
#define TEST_OFF()              (GPIOC->BRR = BIT(PC_TEST))
#define TEST_SET( x )           ((x)? TEST_ON() : TEST_OFF())

#define UCAM_RESET_ON()         (GPIOC->BSRR = BIT(PC_UCAM_RESET))
#define UCAM_RESET_OFF()        (GPIOC->BRR = BIT(PC_UCAM_RESET))
#define UCAM_RESET_SET( x )     ((x)? UCAM_RESET_ON() : UCAM_RESET_OFF())
#define UCAM_USART_DIS()        GPIOA->CRH &= ~GPIO_CRH_CNF9;	\
                                GPIOA->CRH |= GPIO_CRH_CNF9_0;	\
                                GPIOA->CRH &= ~GPIO_CRH_CNF10;	\
                                GPIOA->CRH |= GPIO_CRH_CNF10_0

#define UCAM_USART_EN()         GPIOA->CRH &= ~GPIO_CRH_CNF9_0; \
                                GPIOA->CRH |= GPIO_CRH_CNF9_1;  \
                                GPIOA->CRH |= GPIO_CRH_CNF10_0; \
                                GPIOA->CRH &= ~GPIO_CRH_CNF10_1


#define IN_BTN()                (GPIOC->IDR & BIT(PC_TAC_BTN2))

#define M1_DIR_ON()             (GPIOA->BSRR = BIT(PA_M1_DIR))
#define M1_DIR_OFF()            (GPIOA->BRR = BIT(PA_M1_DIR))
#define M1_PWM_OUT_DIS()        (GPIOB->CRL &= ~GPIO_CRL_CNF6)						
#define M1_PWM_OUT_EN()			GPIOB->CRL &= ~GPIO_CRL_CNF6_0; \
                                GPIOB->CRL |= GPIO_CRL_CNF6_1							
#define M1_PWM_OUT_SET( x )     ((x)? M1_PWM_OUT_EN() : M1_PWM_OUT_DIS())

#define M2_DIR_ON()             (GPIOA->BSRR = BIT(PA_M2_DIR))
#define M2_DIR_OFF()            (GPIOA->BRR = BIT(PA_M2_DIR))
#define M2_PWM_OUT_DIS()        (GPIOB->CRL &= ~GPIO_CRL_CNF7)						
#define M2_PWM_OUT_EN()			GPIOB->CRL &= ~GPIO_CRL_CNF7_0; \
                                GPIOB->CRL |= GPIO_CRL_CNF7_1							
#define M2_PWM_OUT_SET( x )     ((x)? M2_PWM_OUT_EN() : M2_PWM_OUT_DIS())

#define M3_DIR_ON()             (GPIOB->BSRR = BIT(PB_M3_DIR))
#define M3_DIR_OFF()            (GPIOB->BRR = BIT(PB_M3_DIR))
#define M3_PWM_OUT_DIS()        (GPIOB->CRH &= ~GPIO_CRH_CNF8)						
#define M3_PWM_OUT_EN()			GPIOB->CRH &= ~GPIO_CRH_CNF8_0; \
                                GPIOB->CRH |= GPIO_CRH_CNF8_1							
#define M3_PWM_OUT_SET( x )     ((x)? M3_PWM_OUT_EN() : M3_PWM_OUT_DIS())


#define M4_DIR_ON()             (GPIOB->BSRR = BIT(PB_M4_DIR))
#define M4_DIR_OFF()            (GPIOB->BRR = BIT(PB_M4_DIR))
#define M4_PWM_OUT_DIS()        (GPIOB->CRH &= ~GPIO_CRH_CNF9)						
#define M4_PWM_OUT_EN()			GPIOB->CRH &= ~GPIO_CRH_CNF9_0; \
                                GPIOB->CRH |= GPIO_CRH_CNF9_1							
#define M4_PWM_OUT_SET( x )     ((x)? M4_PWM_OUT_EN() : M4_PWM_OUT_DIS())

#define US0_TRIG_ON()           (GPIOD->BSRR = BIT(PD_US0_TRIG))
#define US0_TRIG_OFF()          (GPIOD->BRR = BIT(PD_US0_TRIG))
#define US0_TRIG_SET( x )       ((x)? US0_TRIG_ON() : US0_TRIG_OFF())

#define US1_TRIG_ON()           (GPIOA->BSRR = BIT(PA_US1_TRIG))
#define US1_TRIG_OFF()			(GPIOA->BRR = BIT(PA_US1_TRIG))
#define US1_TRIG_SET( x )		((x)? US1_TRIG_ON() : US1_TRIG_OFF())

#define US2_TRIG_ON()			(GPIOB->BSRR = BIT(PB_US2_TRIG))
#define US2_TRIG_OFF()			(GPIOB->BRR = BIT(PB_US2_TRIG))
#define US2_TRIG_SET( x )		((x)? US2_TRIG_ON() : US2_TRIG_OFF())

#define US3_TRIG_ON()			(GPIOB->BSRR = BIT(PB_US3_TRIG))
#define US3_TRIG_OFF()			(GPIOB->BRR = BIT(PB_US3_TRIG))
#define US3_TRIG_SET( x )		((x)? US3_TRIG_ON() : US3_TRIG_OFF())

#define US4_TRIG_ON()			(GPIOA->BSRR = BIT(PA_US4_TRIG))
#define US4_TRIG_OFF()			(GPIOA->BRR = BIT(PA_US4_TRIG))
#define US4_TRIG_SET( x )		((x)? US4_TRIG_ON() : US4_TRIG_OFF())

#define US5_TRIG_ON()			(GPIOC->BSRR = BIT(PC_US5_TRIG))
#define US5_TRIG_OFF()			(GPIOC->BRR = BIT(PC_US5_TRIG))
#define US5_TRIG_SET( x )		((x)? US5_TRIG_ON() : US5_TRIG_OFF())

#define US6_TRIG_ON()			(GPIOA->BSRR = BIT(PA_US6_TRIG))
#define US6_TRIG_OFF()			(GPIOA->BRR = BIT(PA_US6_TRIG))
#define US6_TRIG_SET( x )		((x)? US6_TRIG_ON() : US6_TRIG_OFF())

#define US7_TRIG_ON()			(GPIOD->BSRR = BIT(PD_US7_TRIG))
#define US7_TRIG_OFF()			(GPIOD->BRR = BIT(PD_US7_TRIG))
#define US7_TRIG_SET( x )		((x)? US7_TRIG_ON() : US7_TRIG_OFF())

#define IN_M1_ENC_INA()			(GPIOA->IDR & BIT(PA_M1_ENC_INA))
#define IN_M1_ENC_INB()			(GPIOC->IDR & BIT(PC_M1_ENC_INB))

#define IN_M2_ENC_INA()			(GPIOC->IDR & BIT(PC_M2_ENC_INA))
#define IN_M2_ENC_INB()			(GPIOC->IDR & BIT(PC_M2_ENC_INB))

#define IN_M3_ENC_INA()			(GPIOC->IDR & BIT(PC_M3_ENC_INA))
#define IN_M3_ENC_INB()			(GPIOC->IDR & BIT(PC_M3_ENC_INB))

#define IN_M4_ENC_INA()			(GPIOA->IDR & BIT(PA_M4_ENC_INA))
#define IN_M4_ENC_INB()			(GPIOC->IDR & BIT(PC_M4_ENC_INB))


/******************************************************************************
 Global functions
******************************************************************************/


/******************************************************************************
 @Description 	: 

 @param			: 
 
 @revision		: 1.0.0
 
******************************************************************************/
void PortInit( void );



#endif /* __HAL_DOT_H__ */









