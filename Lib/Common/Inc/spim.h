/*****************************************************************************
 @Project	: STM32F7XX SPI Master driver
 @File 		: spim.h
 @Details  	: SPI Master hardware layer                    
 @Author	: 
 @Hardware	: 
 
 --------------------------------------------------------------------------
 @Revision	:
  Ver  	Author    	Date        	Changes
 --------------------------------------------------------------------------
   1.0  Name     XXXX-XX-XX  		Initial Release
   
******************************************************************************/


/*****************************************************************************
 Define
******************************************************************************/
#define SPIM_STS_OK					0
#define SPIM_E_INVALID_PORT			-1
#define SPIM_E_INVALID_CLK_POLARITY	-2
#define SPIM_E_INVALID_CLK_EDGE		-3
#define SPIM_E_INVALID_DATASIZE		-4


/*****************************************************************************
 Type definition
******************************************************************************/
typedef enum _SPIM_CFG
{
	SPI_CLK_INACT_HIGH,		/* Clock polarity selection */
	SPI_CLK_INACT_LOW,
	
	SPI_CLK_RISING_EDGE,		/* Clock sampling edge selection */
	SPI_CLK_FALLING_EDGE,
	
	SPI_DATA_SIZE_4,
	SPI_DATA_SIZE_5,
	SPI_DATA_SIZE_6,
	SPI_DATA_SIZE_7,
	SPI_DATA_SIZE_8,
	SPI_DATA_SIZE_9,
	SPI_DATA_SIZE_10,
	SPI_DATA_SIZE_11,
	SPI_DATA_SIZE_12,
	SPI_DATA_SIZE_13,
	SPI_DATA_SIZE_14,
	SPI_DATA_SIZE_15,
	SPI_DATA_SIZE_16
}SPIM_CFG;

/* SPI Driver typedef */
typedef void SPIM_CB_TRFR_DONE( void );

typedef int SPIM_INIT(
				void 		*phandle,
				char 		nPort,
				int 		nSpeed,
				SPIM_CFG 	ClkInactive,
				SPIM_CFG 	ClkEdge,
				SPIM_CFG	DATA_SIZE );

typedef void SPIM_ADD_CB_DONE( void *phandle, SPIM_CB_TRFR_DONE *pfDone );

typedef void SPIM_TRANSFER(
				void 		*phandle,
				void const 	*pTxBuf,
				void 		*pRxBuf,
				int 		nSize );				

typedef struct _TagSPIM_DRV
{
	SPIM_INIT 			*pfInit;
	SPIM_ADD_CB_DONE	*pfAddTransferDone;
	SPIM_TRANSFER		*pfTransfer;
}SPIM_DRV, *PSPIM_DRV;

/*SPI Instance handle */
typedef struct _tagSpim_handle
{
	void 				*pSpi;
	int					Irq;
	
	volatile int		nTxCount;
	volatile int		nRxCount;
	volatile SPIM_DRV	*pDriver;
	volatile char 		*pTxBuf;
	volatile char 		*pRxBuf;
	int 				nSize;
	
	SPIM_CB_TRFR_DONE	*pfDone;
}SPIM_HANDLE, *PSPIM_HANDLE;



















