/**
 ******************************************************************************
  * @file    mb85rsxx.c
  * @brief   This file includes driver for the MB85RS series
  ******************************************************************************
  * @attention
  *
  *  
  * 22.08.2022 - Tested - MB85RS4MTY
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "mb85rsxx.h"

/* Private define ------------------------------------------------------------*/
#define delay(ms) osDelay(ms)
#define get_tick() HAL_GetTick()


/* Private functions ---------------------------------------------------------*/
static void	MB85RSx_Reset(void);
static uint8_t MB85RSx_GetStatus(uint8_t * stat);


/* Private variables ---------------------------------------------------------*/
MB85RSx_Parameter MB85RSx_Para;


/**
  * @brief  Initializes the MB85RSxxxx interface.
  * @retval None
  */
uint8_t MB85RSx_Init(void)
{ 
  uint8_t state;
  /* Reset MB85RSxxxx */
  MB85RSx_Reset();

  state = MB85RSx_Get_Parameter(&MB85RSx_Para);

  return state;
}

/**
  * @brief  This function reset the MB85RSx.
  * @retval None
  */
static void	MB85RSx_Reset(void)
{
  uint8_t cmd[2] = {RESET_ENABLE_CMD,RESET_MEMORY_CMD};

  MB85RSx_Enable();
  /* Send the reset command */
  HAL_SPI_Transmit(&hspi1, cmd, 2, MB85RSxxxx_TIMEOUT_VALUE);	
  MB85RSx_Disable();

}

/**
  * @brief  Reads current status of the MB85RSxxxx.
  * @retval MB85RSxxxx memory status
  */
static uint8_t MB85RSx_GetStatus(uint8_t * stat)
{
  uint8_t cmd[] = {READ_STATUS_REG1_CMD};
  uint8_t status;
  HAL_StatusTypeDef ret;

  MB85RSx_Enable();

  /* Send the read status command */
  ret = HAL_SPI_Transmit(&hspi1, cmd, 1, MB85RSxxxx_TIMEOUT_VALUE);
  
  if(ret == HAL_OK)
  {
    /* Reception of the data */
    ret = HAL_SPI_Receive(&hspi1,&status, 1, MB85RSxxxx_TIMEOUT_VALUE);
  }
  
  MB85RSx_Disable();
	
  /* Check the value of the register */
  if( ret != 0 )
  {
    return MB85RSx_BUSY;
  }
  else
  {
    *stat = status;
    return MB85RSx_OK;
  }
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @retval None
  */
uint8_t MB85RSx_WriteEnable(void)
{
  uint8_t cmd[] = {WRITE_ENABLE_CMD};
  uint32_t tickstart = get_tick();
  uint8_t stat;

  /*Select the FLASH: Chip Select low */
  MB85RSx_Enable();
  /* Send the read ID command */
  HAL_SPI_Transmit(&hspi1, cmd, 1, MB85RSxxxx_TIMEOUT_VALUE);	
  /*Deselect the FLASH: Chip Select high */
  MB85RSx_Disable();

  /* Wait the end of Flash writing */
  while(MB85RSx_GetStatus(&stat) == MB85RSx_BUSY)
  {
    /* Check for the Timeout */
    if((get_tick() - tickstart) > MB85RSxxxx_TIMEOUT_VALUE)
    {        
      return MB85RSx_TIMEOUT;
    }
    delay(1);
  }

  return MB85RSx_OK;
}

/**
  * @brief  Read Manufacture/Device ID.
  * @param  return value address
  * @retval None
  */
uint32_t MB85RSx_Read_ID(void)
{
  uint8_t idt[4] = {0,0,0,0};
  uint32_t id;

  uint8_t cmd[4] = {READ_ID_CMD,0x00,0x00,0x00};

  MB85RSx_Enable();
  
  
  /* Send the read ID command */
  if(!HAL_SPI_Transmit(&hspi1, cmd, 1, MB85RSxxxx_TIMEOUT_VALUE))
  {
    /* Reception of the data */
    HAL_SPI_Receive(&hspi1,idt, 4, MB85RSxxxx_TIMEOUT_VALUE);
  }

  id = (idt[0] << 24) + (idt[1] << 16) + (idt[2] << 8) + idt[3];

  MB85RSx_Disable();
  
  return id;
		
}

#include <math.h>
/**
  * @brief  Get MB85RSX Parameter.
  * @param  Para: MB85RSx_Parameter
  * @retval NULL
  */
uint8_t MB85RSx_Get_Parameter(MB85RSx_Parameter *Para)
{
	uint32_t id;
	uint32_t size;
        uint32_t density;
        uint32_t type;
        uint8_t status;
	
	Para->PAGE_SIZE = 1;
        
        if(MB85RSx_GetStatus(&status) == MB85RSx_OK)
        {
          id = MB85RSx_Read_ID();
          type = (~0xFFFF0000  & (id >> 16));
          switch (type)
          {
            case MB85RS80:
            case MB85RS16:
            case MB85RS32:
            case MB85RS64:
            case MB85RS128:
              size = (uint32_t) powf(2,(id - 0xEF13)) * 1024 * 1024;
              break;
            case MB85RSxM:
              
              density = (~0xFFFFFF00  & (id >> 8));
              
              if(density == 0x49)
              {
                size = 8 * (0x10000);
                MB85RSx_Para.SECTOR_COUNT = MB85RS4M_SECTOR_COUNT;
                MB85RSx_Para.SECTOR_SIZE = MB85RS4M_SECTOR_SIZE;
                MB85RSx_Para.SUBSECTOR_COUNT = MB85RS4M_SUBSECTOR_COUNT;
                MB85RSx_Para.SUBSECTOR_SIZE = MB85RS4M_SUBSECTOR_SIZE;
                MB85RSx_Para.PAGE_SIZE = MB85RS4M_PAGE_SIZE;
              }
              else if( 0x08)
              {
                size = 2 * (0x10000);
              }
              else if(0x27)
              {
                size = 1 * (0x10000);
              }
              else
              {
                size = 0x1000;
              }
              
              MB85RSx_Para.THREE_BYTE_ADDRESS = 1;
              
              Para->THREE_BYTE_ADDRESS  = 1;
              break;
          default:
            return MB85RSx_ERROR;
          }
        }
        else
        {
          return MB85RSx_ERROR;
        }
          
	
	
	
	Para->FLASH_ID = id;
	Para->FLASH_SIZE = size;
	
	return MB85RSx_OK;
}
/**
  * @brief  Reads an amount of data from the QSPI memory.
  * @param  pData: Pointer to data to be read
  * @param  ReadAddr: Read start address
  * @param  Size: Size of data to read    
  * @retval QSPI memory status
  */
uint8_t MB85RSx_Read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  uint8_t cmd[4];
  uint16_t cmdSize;
  HAL_StatusTypeDef stat;
  uint32_t timeout = MB85RSxxxx_TIMEOUT_VALUE;

  if( (ReadAddr + Size) >= MB85RSx_Para.FLASH_SIZE)
    return MB85RSx_ERROR;
  
  /* Configure the command */
  cmd[0] = READ_CMD;
  if(MB85RSx_Para.THREE_BYTE_ADDRESS)
  {
    cmd[1] = (uint8_t)(ReadAddr >> 16);
    cmd[2] = (uint8_t)(ReadAddr >> 8);
    cmd[3] = (uint8_t)(ReadAddr);
    cmdSize = 4;
  }
  else
  {
    cmd[1] = (uint8_t)(ReadAddr >> 8);
    cmd[2] = (uint8_t)(ReadAddr);
    cmdSize = 3;
  }
  
  if(Size > 0x1000)
  {
    timeout = (uint32_t)(Size/0x1000)*MB85RSxxxx_TIMEOUT_VALUE;
  }

  MB85RSx_Enable();
  /* Send the read ID command */
  stat = HAL_SPI_Transmit(&hspi1, cmd, cmdSize, MB85RSxxxx_TIMEOUT_VALUE);
  
  if( stat == HAL_OK )
  {
    /* Reception of the data */
    stat = HAL_SPI_Receive(&hspi1, pData,Size,timeout);
    if ( stat != HAL_OK)
    {
      return MB85RSx_ERROR;
    }
  }
  MB85RSx_Disable();
  return MB85RSx_OK;
}

/**
  * @brief  Writes an amount of data to the QSPI memory.
  * @param  pData: Pointer to data to be written
  * @param  WriteAddr: Write start address
  * @param  Size: Size of data to write,No more than 256byte.    
  * @retval QSPI memory status
  */
uint8_t MB85RSx_Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
  uint8_t cmd[4];
  uint32_t current_size, current_addr;
  uint32_t tickstart = get_tick();
  uint16_t cmdSize;
  

  /* Initialize the adress variables */
  current_addr = WriteAddr;
  current_size = Size;
  
  if( (WriteAddr + Size) >= MB85RSx_Para.FLASH_SIZE)
    return MB85RSx_ERROR;
  
  /* Configure the command */
  cmd[0] = WRITE_CMD;
  if(MB85RSx_Para.THREE_BYTE_ADDRESS)
  {
    cmd[1] = (uint8_t)(current_addr >> 16);
    cmd[2] = (uint8_t)(current_addr >> 8);
    cmd[3] = (uint8_t)(current_addr);
    cmdSize = 4;
  }
  else
  {
    cmd[1] = (uint8_t)(current_addr >> 8);
    cmd[2] = (uint8_t)(current_addr);
    cmdSize = 3;
  }
  
  /* Enable write operations */
  if(!MB85RSx_WriteEnable())
  {
    MB85RSx_Enable();
    /* Send the command */
    if (HAL_SPI_Transmit(&hspi1,cmd, cmdSize, MB85RSxxxx_TIMEOUT_VALUE) != HAL_OK)
    {
      return MB85RSx_ERROR;
    }

    /* Transmission of the data */
    if (HAL_SPI_Transmit(&hspi1, pData,current_size, MB85RSxxxx_TIMEOUT_VALUE) != HAL_OK)
    {
      return MB85RSx_ERROR;
    }
    MB85RSx_Disable();
  }
  else
    return MB85RSx_ERROR;

  return MB85RSx_OK;
}

/**
  * @brief  Erases the specified block of the QSPI memory. 
  * @param  BlockAddress: Block address to erase  
  * @retval QSPI memory status
  */
uint8_t MB85RSx_Erase_Block(uint32_t Address)
{
//  uint8_t cmd[4];
//  uint32_t tickstart = get_tick();
//  cmd[0] = SECTOR_ERASE_CMD;
//  cmd[1] = (uint8_t)(Address >> 16);
//  cmd[2] = (uint8_t)(Address >> 8);
//  cmd[3] = (uint8_t)(Address);
//
//  /* Enable write operations */
//  MB85RSx_WriteEnable();
//
//  /*Select the FLASH: Chip Select low */
//  MB85RSx_Enable();
//  /* Send the read ID command */
//  HAL_SPI_Transmit(&hspi1, cmd, 4, MB85RSxxxx_TIMEOUT_VALUE);	
//  /*Deselect the FLASH: Chip Select high */
//  MB85RSx_Disable();
//
//  /* Wait the end of Flash writing */
//  while(MB85RSx_GetStatus() == MB85RSx_BUSY)
//  {
//    /* Check for the Timeout */
//    if((get_tick() - tickstart) > MB85RSxxxx_SECTOR_ERASE_MAX_TIME)
//    {        
//      return MB85RSx_TIMEOUT;
//    }
//    //delay(1);
//  }
  return MB85RSx_OK;
}

/**
  * @brief  Erases the entire QSPI memory.This function will take a very long time.
  * @retval QSPI memory status
  */
uint8_t MB85RSx_Erase_Chip(void)
{
//  uint8_t cmd[4];
//  uint32_t tickstart = get_tick();
//  cmd[0] = CHIP_ERASE_CMD;
//
//  /* Enable write operations */
//  MB85RSx_WriteEnable();
//
//  /*Select the FLASH: Chip Select low */
//  MB85RSx_Enable();
//  /* Send the read ID command */
//  HAL_SPI_Transmit(&hspi1, cmd, 1, MB85RSxxxx_TIMEOUT_VALUE);	
//  /*Deselect the FLASH: Chip Select high */
//  MB85RSx_Disable();
//
//  /* Wait the end of Flash writing */
//  while(MB85RSx_GetStatus() == MB85RSx_BUSY)
//  {
//    /* Check for the Timeout */
//    if((get_tick() - tickstart) > MB85RSxxxx_BULK_ERASE_MAX_TIME)
//    {        
//      return MB85RSx_TIMEOUT;
//    }
//  //delay(1);
//  }
  return MB85RSx_OK;
}


