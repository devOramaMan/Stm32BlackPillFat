/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MB85RSXX_H
#define __MB85RSXX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "spi.h"
	 
/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup MB85RSxxxx
  * @{
  */

/** @defgroup MB85RSxxxx_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup MB85RSxxxx_Exported_Constants
  * @{
  */
   
/** 
  * @brief  MB85RSXX Configuration  
  */  
	
/* MB85RS4M */

  

// TODO MOVE TO DEFINITION
#define MB85RS4M_SIZE                        0x80000
#define MB85RS4M_SECTOR_COUNT                0x08      
#define MB85RS4M_SECTOR_SIZE                 0x10000   
#define MB85RS4M_SUBSECTOR_COUNT             0x80    
#define MB85RS4M_SUBSECTOR_SIZE              0x1000  
#define MB85RS4M_PAGE_SIZE                   0x100
   
#if (USE_MB85RS4M == 1)
  #define MB85RSxxxx_FLASH_SIZE                  MB85RS4M_SIZE
#endif

#define MB85RS80  0xEF13
#define MB85RS16  0xEF14
#define MB85RS32  0xEF15
#define MB85RS64  0xEF16
#define MB85RS128 0xEF17
#define MB85RSxM  0x047F
    

#define MB85RSxxxx_DUMMY_CYCLES_READ           4
#define MB85RSxxxx_DUMMY_CYCLES_READ_QUAD      10

#define MB85RSxxxx_BULK_ERASE_MAX_TIME         250000
#define MB85RSxxxx_SECTOR_ERASE_MAX_TIME       3000
#define MB85RSxxxx_SUBSECTOR_ERASE_MAX_TIME    800
#define MB85RSxxxx_TIMEOUT_VALUE               1000

/** 
  * @brief  MB85RSXX Commands  
  */  

// Status register
#define MB85RSxxxx_WPEN       0x80 // Status Register Write Protect
#define MB85RSxxxx_BP1        0x08 // Block Protect 1
#define MB85RSxxxx_BP0        0x04 // Block Protect 0
#define MB85RSxxxx_WEL        0x02 // Write Enable Latch

// OP-CODE
#define MB85RSxxxx_WREN       0x06 // Set Write Enable Latch   0000 0110B   
#define MB85RSxxxx_WRDI       0x04 // Reset Write Enable Latch 0000 0100B 
#define MB85RSxxxx_RDSR       0x05 // Read Status Register     0000 0101B     
#define MB85RSxxxx_WRSR       0x01 // Write Status Register    0000 0001B    
#define MB85RSxxxx_READ       0x03 // Read Memory Code         0000 0011B         
#define MB85RSxxxx_WRITE      0x02 // Write Memory Code        0000 0010B  
#define MB85RSxxxx_FSTRD      0x0B // Fast Read Memory Code    0000 1011B
#define MB85RSxxxx_DPD        0xBA // Deep Power Down Mode     1011 1010B     
#define MB85RSxxxx_HIBERNATE  0xB9 // Hibernate Mode           1011 1001B
#define MB85RSxxxx_RDID       0x9F // Read Device ID           1001 1111B
#define MB85RSxxxx_RUID       0x4C // Read Unique ID           0100 1100B
#define MB85RSxxxx_WRSN       0xC2 // Write Serial Number      1100 0010B
#define MB85RSxxxx_RDSN       0xC3 // Read Serial Number       1100 0011B
#define MB85RSxxxx_SSWR       0x42 // Write Special Sector     0100 0010B
#define MB85RSxxxx_SSRD       0x4B // Read Special Sector      0100 1011B
#define MB85RSxxxx_FSSRD      0x49 // Fast Read Special Sector 0100 1001B



#define MB85RSxxxx_MEM_SIZE_SMALL   0x8000	//Memory size
#define MB85RSxxxx_MEM_SIZE_BIG     0x20000


/** 
  * @brief  MB85RSXX Commands  
  */  
/* Reset Operations */
#define RESET_ENABLE_CMD                     MB85RSxxxx_WREN
#define RESET_MEMORY_CMD                     MB85RSxxxx_WRDI

//#define ENTER_QPI_MODE_CMD                   0x38
//#define EXIT_QPI_MODE_CMD                    0xFF

/* Identification Operations */
#define READ_ID_CMD                          MB85RSxxxx_RDID
//#define DUAL_READ_ID_CMD                     0x92
//#define QUAD_READ_ID_CMD                     0x94
//#define READ_JEDEC_ID_CMD                    0x9F

/* Read Operations */
#define READ_CMD                              MB85RSxxxx_READ
#define WRITE_CMD                             MB85RSxxxx_WRITE
#define FAST_READ_CMD                         MB85RSxxxx_FSTRD
//#define DUAL_OUT_FAST_READ_CMD               0x3B
//#define DUAL_INOUT_FAST_READ_CMD             0xBB
//#define QUAD_OUT_FAST_READ_CMD               0x6B
//#define QUAD_INOUT_FAST_READ_CMD             0xEB

/* Write Operations */
#define WRITE_ENABLE_CMD                      MB85RSxxxx_WREN
#define WRITE_DISABLE_CMD                     MB85RSxxxx_WRDI

/* Register Operations */
#define READ_STATUS_REG1_CMD                  MB85RSxxxx_RDSR
//#define READ_STATUS_REG2_CMD                  0x35
//#define READ_STATUS_REG3_CMD                  0x15

#define WRITE_STATUS_REG1_CMD                 MB85RSxxxx_WRSR
//#define WRITE_STATUS_REG2_CMD                 0x31
//#define WRITE_STATUS_REG3_CMD                 0x11


#define MB85RSx_Enable() 		      HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET)
#define MB85RSx_Disable() 		      HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET)

#define MB85RSx_OK            ((uint8_t)0x00)
#define MB85RSx_ERROR         ((uint8_t)0x01)
#define MB85RSx_BUSY          ((uint8_t)0x02)
#define MB85RSx_TIMEOUT	      ((uint8_t)0x03)

typedef struct
{
	uint32_t FLASH_ID;
	uint32_t FLASH_SIZE;        
	uint32_t SECTOR_COUNT;
	uint32_t SECTOR_SIZE;
	uint32_t SUBSECTOR_COUNT;
	uint32_t SUBSECTOR_SIZE;
	uint32_t PAGE_SIZE;
        uint32_t THREE_BYTE_ADDRESS;
}MB85RSx_Parameter;

extern MB85RSx_Parameter MB85RSx_Para;

uint8_t MB85RSx_Init(void);
uint8_t MB85RSx_WriteEnable(void);
uint32_t MB85RSx_Read_ID(void);
uint8_t MB85RSx_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);
uint8_t MB85RSx_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
uint8_t MB85RSx_Erase_Block(uint32_t Address);
uint8_t MB85RSx_Erase_Chip(void);

uint8_t MB85RSx_Get_Parameter(MB85RSx_Parameter *Para);
/**
  * @}
  */
  
/** @defgroup MB85RSxxxx_Exported_Functions
  * @{
  */ 
/**
  * @}
  */ 
      
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __MB85RSx_H */

