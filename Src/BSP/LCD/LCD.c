/*****************************************************************************
 @Project	: 
 @File 		: LCD.c
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
#include "i2c.h"
#include "LCD.h"


/*****************************************************************************
 Define
******************************************************************************/

/* LCD control bits from PCF8574   */
#define     LCD_RS      1 << 0      /* RS=1 for Data; RS=0 for Control Word */
#define     LCD_RW      1 << 1      /* RW=0 for Write; RW=1 for Read        */
#define     LCD_E      1 << 2      /* CS=0 for Chip Select                 */
#define     LCD_K       1 << 3      /* LED cathode                          */

#define     LCD_D(n)    1 << n

#define LCD_D_HNIBBLE(n)                 (n & 0xF0)
#define LCD_D_LNIBBLE(n)                  ((n & 0x0F) << 4)

/* LCD REGISTERS */
#define LCD_CONTROL                     0
#define LCD_DATA                        LCD_RS


/* LCD_INSTRUCTIONS */
#define LCD_CLEAR_DISPLAY               0x01

#define LCD_RETURN_HOME                 0x02

#define LCD_EMS                         0x04    /* ENTRY MODE SET       */
#define LCD_EMS_S                       0x01    /* Shift                */
#define LCD_EMS_ID                      0x02    /* Increment position   */

#define LCD_DC                          0x08    /* DISPLAY CONTROL      */
#define LCD_DC_B                        0x01    /* Blink Cursor         */
#define LCD_DC_C                        0x02    /* Display Cursor       */
#define LCD_DC_D                        0x04    /* Display OM           */

#define LCD_CDS                         0x10    /* CURSOR OR DISPLAY SHIFT      */
#define LCD_CDS_RL                      0x04    /* 1=Right; 0=Left              */
#define LCD_CDS_SC                      0x08    /* 1=Screen; 0=Cursor           */

#define LCD_FS                          0x20    /* FUNCTION SET                 */
#define LCD_FS_F                        0x04    /* 1=5X10 dots; 0=5X7 dots      */
#define LCD_FS_N                        0x08    /* 1=2 lines; 0=1 line display  */
#define LCD_FS_DL                       0x10    /* 1=8-bit data; 0=4-bit data   */

#define LCD_SET_CGRAM_ADDRESS           0x40    
#define LCD_SET_CGRAM_ADDRESS_MASK      0X3F

#define LCD_SET_DDRAM_ADDRESS           0x80    /* SET CURSOR POSITION          */
#define LCD_SET_DDRAM_ADDRESS_MASK      0X7F    

#define LCD_BUSY_FLAG                   0X80


/*****************************************************************************
 Type definition
******************************************************************************/


/*****************************************************************************
 Global Variables
******************************************************************************/


void LCD_init();
void LCD_Write_Control(uint8_t);
void LCD_Write_Data (uint8_t data);
void Write_Data_Sequence(uint8_t);
void Write_Ctrl_Sequence(uint8_t);

extern void Delayms(int);

static void lcd_cbI2cDone( void );

/*****************************************************************************
 Local Variables
******************************************************************************/

static PI2C_HANDLE g_pI2cHandle = 0;
static I2C_HOOK                         g_I2cHook;
static volatile int                     g_bI2cDone = FALSE;

static uint8_t w_data[7] = {0,0,0,0,0,0,0};


/*****************************************************************************
 Implementation
******************************************************************************/

void LCD_init( PI2C_HANDLE pHandle )
{
    g_pI2cHandle = pHandle;
    
    I2cAddHook(
        g_pI2cHandle,
        &g_I2cHook,
        lcd_cbI2cDone );

    //Delayms(15);
    Write_Ctrl_Sequence(0x03);
    
    Write_Ctrl_Sequence(0x02);
   
    Write_Ctrl_Sequence(0b00101000); // function set: 4-bit data, 2 lines, 5X7
   
    Write_Ctrl_Sequence(0b00001100); // display on, cursor off, char no blink
    Write_Ctrl_Sequence(0b00001110); // display on, cursor on, char no blink
   
    Write_Ctrl_Sequence(0b00000110); // entry mode: incr add, shift R
      
    Write_Ctrl_Sequence(0b00000001); // clear display, cursor home
    TRACE( "Clear LCD Display\r\n");
}




void Write_Data_Sequence(uint8_t data)
{
    memset( w_data, 0, sizeof(w_data) );
/*
    w_data[0] &= ~LCD_RW; // R/W=0
    w_data[0] |= LCD_RS;   //RS = 1
    w_data[0] |= LCD_K;
    //w_data[0] &= ~LCD_K;
    
    w_data[1] = w_data[0];
    w_data[1] |= LCD_E;  // LCD_E = 1
   
    w_data[2] = w_data[1]; 
    //w_data[2] |= LCD_D_HNIBBLE(data);  // LCD_HDATA
    w_data[2] |= 0b01000000;
  
    
    w_data[3] = w_data[2];
    w_data[3] &= ~LCD_E;  // LCD_E=0
   
  

    w_data[4] = w_data[3];
    w_data[4] |= LCD_E;
   
    w_data[5] = w_data[4];
   w_data[5] &= ~0b11110000;
   //w_data[5] |= LCD_D_LNIBBLE(data);
    w_data[5] |= 0b00010000;

    w_data[6] = w_data[5];
    w_data[6] &= ~LCD_E;
*/

    w_data[0] = 0b00001001;
    w_data[1] = 0b00001101;// E=1

    w_data[2] = w_data[1];
    //TRACE( "HNibble=%i\r\n",LCD_D_HNIBBLE(data) );
    w_data[2] |= LCD_D_HNIBBLE(data);
    
    w_data[3] = 0b01001001;  //E=0
    w_data[4] = 0b00001101; //E=1
    
    //TRACE( "LNibble=%i\r\n",LCD_D_LNIBBLE(data) );
    w_data[5] = w_data[4];
    w_data[5] |= LCD_D_LNIBBLE(data);

    w_data[6] = 0b00011001; // E=0

    I2cWrite( g_pI2cHandle, LCD_slave_addr, &w_data[0] , &w_data[1],  6U);  /*pHandle, nSlaveAddr, Register, *pData, nByte   */
    while( FALSE == g_bI2cDone);
    g_bI2cDone = FALSE;
}

void Write_Ctrl_Sequence( uint8_t data)
{
    memset( w_data, 0, sizeof(w_data) );
    //TRACE( "Ctrl Seq\r\n" );
    /*
    w_data[0] &= ~LCD_RW; // RW=0
    w_data[0] &= ~LCD_RS; // RS=0
    w_data[0] |= LCD_K;  // LED= ON
   

    w_data[1] = w_data[0];
    w_data[1] |= LCD_E;  // LCD_E = 1
   
    w_data[2] = w_data[1]; 
    w_data[2] |= LCD_D_HNIBBLE(data);  // LCD_HDATA
    
  

    w_data[3] = w_data[2];
    w_data[3] &= ~LCD_E;  // LCD_E=0
   
  
    w_data[4] = w_data[3];
    w_data[4] |= LCD_E;
   
    w_data[5] = w_data[4];
    w_data[5] |= LCD_D_LNIBBLE(data);
    
    w_data[6] = w_data[5];
    w_data[6] &= ~LCD_E;
    */
    
    w_data[0] = 0b00001000;
    w_data[1] = 0b00001100;// E=1

    w_data[2] = w_data[1];
    //TRACE( "HNibble=%i\r\n",LCD_D_HNIBBLE(data) );
    w_data[2] |= LCD_D_HNIBBLE(data);
    
    w_data[3] = 0b01001000;  //E=0
    w_data[4] = 0b00001100; //E=1
    
    //TRACE( "LNibble=%i\r\n",LCD_D_LNIBBLE(data) );
    w_data[5] = w_data[4];
    w_data[5] |= LCD_D_LNIBBLE(data);

    w_data[6] = 0b00011000; // E=0

    I2cWrite( g_pI2cHandle, LCD_slave_addr, &w_data[0] , &w_data[1],  6U);  /*pHandle, nSlaveAddr, Register, *pData, nByte   */
    while( FALSE == g_bI2cDone);
    g_bI2cDone = FALSE;
}

void LCD_Write_Data ( uint8_t data)
{
    Write_Data_Sequence(data);
      
   // I2cWrite( g_pI2cHandle, LCD_slave_addr, &w_data[0] , &w_data[1],  6U);  /*pHandle, nSlaveAddr, Register, *pData, nByte   */
   // while( FALSE == g_bI2cDone);
   // g_bI2cDone = FALSE;
}
  


static void lcd_cbI2cDone( void )
{
    g_bI2cDone = TRUE;
}
