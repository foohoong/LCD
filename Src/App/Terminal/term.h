/*****************************************************************************
 @Project	: 
 @File 		: term.h
 @Details  	: 
 @Author	: 
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/

#ifndef __TERM_DOT_H__
#define __TERM_DOT_H__


/*****************************************************************************
 Define
******************************************************************************/
#define CMD_STX						0x30
#define CMD_ETX						0x31		
#define CMD_HOST_PKT_SIZE           10U	

#define RESP_DEV_CUR_XY				0x01
#define RESP_DEV_ACK				0x02
#define RESP_DEV_STS				0x03
#define RESP_DEV_IMU				0x04

/* PC Host terminal command sets */
#define CMD_HOST_START				0x01 
#define CMD_HOST_STOP				0x02 
#define CMD_HOST_MAP_SET			0x03 
#define CMD_HOST_SYS_RESET			0x04 
#define CMD_HOST_SET_REF			0x05 
#define CMD_HOST_SET_CAL_MODE		0x06 
#define CMD_HOST_SAVE				0x07


#define CMD_HOST_MOVE_FWD			0x30 
#define CMD_HOST_MOVE_BWD			0x31 
#define CMD_HOST_ROTATE_LEFT		0x32 
#define CMD_HOST_MOVE_FR			0x33 
#define CMD_HOST_MOVE_RIGHT			0x34 
#define CMD_HOST_MOVE_BR			0x35 
#define CMD_HOST_ROTATE_RIGHT		0x36 
#define CMD_HOST_MOVE_BL			0x37 
#define CMD_HOST_MOVE_LEFT			0x38 
#define CMD_HOST_MOVE_FL			0x39


/*****************************************************************************
 Type definition
******************************************************************************/
#pragma push
#pragma pack(1)
typedef  union _tagDev_RESP_PKT
{
    uint8_t aResp[18];

    struct
    {
        uint8_t STX;
        uint8_t Cmd;
        uint8_t MoveState;
        int32_t nCurX;
        int32_t nCurY;
        uint8_t nPoint;
        int32_t nYaw;
        uint8_t bStop :1;
        uint8_t Runtime :7;
        uint8_t ETX;
    }Info;

    struct
    {
        uint8_t STX;
        uint8_t Cmd;
        int32_t nPitch;
        int32_t nRoll;
        int32_t nYaw;
		uint8_t Resv1;
        uint8_t Resv2;
        uint8_t Resv3;
        uint8_t ETX;
    }Imu;
}DEV_RESP_PKT, *PDEV_RESP_PKT;

#pragma pop

/*****************************************************************************
 Macro
******************************************************************************/


/******************************************************************************
 Global functions
******************************************************************************/


/******************************************************************************
 @Description 	: 

 @param			: 
 
 @revision		: 1.0.0
 
******************************************************************************/
void TerminalInit( void );


/******************************************************************************
 @Description 	: 

 @param			: 
 
 @revision		: 1.0.0
 
******************************************************************************/
char *TerminalParse( UART_HANDLE *pUartHandle );


/******************************************************************************
 @Description 	: 

 @param			: 
 
 @revision		: 1.0.0
 
******************************************************************************/
void TerminalAckToHost( UART_HANDLE *pUartHandle );


#endif /* __TERM_DOT_H__ */









