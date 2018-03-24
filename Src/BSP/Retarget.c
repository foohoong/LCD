/*****************************************************************************
 @Project	: 
 @File 		: Retarget.c
 @Details  	: Re-target printf to IDE debug viewer. Edit from MDK-KEIL sample           
 @Author	: lcgan
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/
#include <Common.h>
#include <debugio.h>
#include "Serial.h"


/*****************************************************************************
 Define
******************************************************************************/
#define LF		0x0a
#define CR		0x0d

#define _SEGGER_TRACE

/*****************************************************************************
 Type definition
******************************************************************************/


/*****************************************************************************
 Global Variables
******************************************************************************/
extern UART_HANDLE		g_DebugSerialHandle;
volatile int32_t 		ITM_RxBuffer;


/*****************************************************************************
 Implementation
******************************************************************************/
char GetKey( void )
{
#ifdef _UART_TRACE
	return SerialIsRxReady( &g_DebugSerialHandle );
#else
    #ifdef _SEGGER_TRACE
        return debug_kbhit();
    #else
        return ITM_CheckChar();
    #endif
#endif
}


int __putchar(int ch )
{
#ifdef _UART_TRACE
	while( 0 == SerialIsTxReady( &g_DebugSerialHandle ) );
	if( LF == ch )
	{
		SerialWrite( &g_DebugSerialHandle, CR );
	}
	return SerialWrite( &g_DebugSerialHandle, ch );
#else 
    #ifdef _SEGGER_TRACE
        if( LF == ch )
        {
            debug_putchar( CR );
        }
        return debug_putchar( ch );
    #else
        if( LF == ch )
        {
            ITM_SendChar( CR );
        }
        return ITM_SendChar( ch );
    #endif
#endif
}


int __getchar( void )
{
#ifdef _UART_TRACE
	while( 0 == SerialIsRxReady( &g_DebugSerialHandle ) );
	return SerialRead( &g_DebugSerialHandle );
#else
    #ifdef _SEGGER_TRACE
        return debug_getch();
    #else
        while( 0 == ITM_CheckChar() );		/* Wait for character to arrive */
        return ITM_ReceiveChar();
    #endif
#endif
}

void FlushInBuf( void )
{
#ifdef _UART_TRACE
    SerialRxEmpty( &g_DebugSerialHandle );
#endif
}



































