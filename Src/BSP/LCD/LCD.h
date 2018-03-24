#include <Common.h>

#include "Hal.h"
#include "Timer.h"
#include "i2c.h"


#define  LCD_slave_addr  0x4E


void LCD_init( PI2C_HANDLE pHandle );
void LCD_Write_Control(uint8_t);
void LCD_Write_Data (uint8_t data);
