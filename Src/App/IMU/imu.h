/*****************************************************************************
 @Project	: 
 @File 		: imu.h
 @Details  	:              
 @Author	: 
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/


#ifndef __IMU_DOT_H__
#define __IMU_DOT_H__

/*****************************************************************************
 Define
******************************************************************************/
#define RAD(x) ((x)*0.01745329252)  /* *pi/180 */
#define ToDeg(x) ((x)*57.2957795131)  /* *180/pi */


/*****************************************************************************
 Type definition
******************************************************************************/


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
void ImuInit( int nSampleFreq );


/******************************************************************************
 @Description 	: 

 @param			: gx, gy, gz = degree/s
 
 @revision		: 1.0.0
 
******************************************************************************/
void ImuUpdateAcclGyro(
			float	gx,
			float	gy,
			float	gz,
			float	ax,
			float	ay,
			float	az );


/******************************************************************************
 @Description 	: 

 @param			: 
 
 @revision		: 1.0.0
 
******************************************************************************/
void ImuUpdate(
			float gx,
			float gy,
			float gz,
			float ax,
			float ay,
			float az,
			float mx,
			float my,
			float mz );


/******************************************************************************
 @Description 	: 

 @param			: 
 
 @revision		: 1.0.0
 
******************************************************************************/
void ImuUpdateAcclGyro(
			float	gx,
			float	gy,
			float	gz,
			float	ax,
			float	ay,
			float	az );


/******************************************************************************
 @Description 	: 

 @param			: 
 
 @revision		: 1.0.0
 
******************************************************************************/
void ImuComputeResult( float *pdRoll, float *pdPitch, float *pdYaw );

#endif /* __IMU_DOT_H__ */