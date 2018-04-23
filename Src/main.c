/*****************************************************************************
 @Project	: STN32F103RB to I2C 16X2 LCD 
 @File 		: main.c
 @Details  	: Main entry               
 @Author	: Fong FH
 @Hardware	: STM32
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Fong FH     1 Mar 2018  		Initial Release
   
******************************************************************************/
#include <Common.h>
#include "Hal.h"
#include "IRQ.h"
#include "BSP.h"
#include "Timer.h"
#include "i2c.h"
#include "LCD.h"

/*****************************************************************************
 Define
******************************************************************************/
#define SYS_TICK_MS         100U

#define TIMER1_TICK_HZ		10000U


/*****************************************************************************
 Type definition
******************************************************************************/
#pragma push
#pragma pack(1)

#pragma pop


/*****************************************************************************
 Global Variables
******************************************************************************/



/*****************************************************************************
 Const Local Variables
******************************************************************************/


/*****************************************************************************
 Local Variables
******************************************************************************/

static TIMER_HANDLE     g_Timer1Handle;

static volatile int     g_nSystemTick = SYS_TICK_MS;
static volatile BOOL    g_bSystemTick = FALSE;

static BOOL             toggle = FALSE;

static I2C_HANDLE       g_I2cHandle;

static char             LCD [2][16] =   {"DIGIPEN INST   1",
                                         "OF TECHNOLOGY  2"};

/*****************************************************************************
 Local function Prototype
******************************************************************************/

static void main_I2cInit( void );
void Delayms( int nMs );

/*****************************************************************************
 Callback function Prototype
******************************************************************************/
static void main_cbI2cDone( void );

/*****************************************************************************
 Implementation 
******************************************************************************/
void main( void )
{
    int i;
    char row;

    BSPInit();
    SysTick_Config( SystemCoreClock/1000 );  /* Generate interrupt each 1 ms */
    main_I2cInit();
    TimerInit( &g_Timer1Handle, 1U, TIMER1_TICK_HZ );  /* 10KHz */
    IRQInit();

    LCD_init( &g_I2cHandle );

    TRACE( "LCD\r\n" );
  
    for (row=0; row<=1; row++)
    {
        for (i=0;i<=15;i++)
           LCD_Write_Data( LCD[row][i] );
        Write_Ctrl_Sequence(0b11000000);  /* position cursor to the 2nd line */
     }

    for(;;)
    {	
        if (0 != g_bSystemTick)
        {            
            g_bSystemTick = FALSE;
            toggle = !toggle;
            LED_SET( toggle );	
            TRACE( "LED blink\r\n" );
        }	        
    }
}


/*****************************************************************************
 Callback functions
******************************************************************************/


/*****************************************************************************
 Local functions
******************************************************************************/
static void main_I2cInit( void )
{
	int res;
		
	res = I2cInit( &g_I2cHandle, 1, 100000U ); /* 0 = I2C1; 1 = I2C2  */
    ASSERT( I2C_STS_OK == res );

}



void Delayms( int nMs )
{
	volatile int cnt = (SystemCoreClock*nMs) / 10000;
	
	while( cnt-- )
	{
		__NOP();
	}
}


/*****************************************************************************
 Interrupt functions
******************************************************************************/
void SysTick_Handler( void )
{
    /* Provide system tick */
    if( 0 != g_nSystemTick )
    {
        g_nSystemTick--;
        if( 0 == g_nSystemTick )
        {
           g_nSystemTick = SYS_TICK_MS;
           g_bSystemTick = TRUE;
        }
    }
}


/*************************** End of file *************************************/
