/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * @file    mb85rsxx_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
 /* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future.
 * Kept to ensure backward compatibility with previous CubeMx versions when
 * migrating projects.
 * User code previously added there should be copied in the new user sections before
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"

#include "mb85rsxx.h"
#include "user_diskio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Disk status */

static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
  /* USER CODE BEGIN INIT */

  return MB85RSx_Init();
  /* USER CODE END INIT */
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
    Stat = RES_OK;
    return Stat;
  /* USER CODE END STATUS */
}

/**
  * @brief  USER_read(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
  uint32_t i;
  for(i = 0; i < count; i ++)
  {
    MB85RSx_Read(buff,sector * (MB85RSx_Para.SUBSECTOR_SIZE),MB85RSx_Para.SUBSECTOR_SIZE);
    sector++;
    buff += MB85RSx_Para.SUBSECTOR_SIZE;
  }
  return RES_OK;
  /* USER CODE END READ */
}

/**
  * @brief  USER_write(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{
  /* USER CODE BEGIN WRITE */
  /* USER CODE HERE */   
  uint32_t i;
  for(i = 0; i < count; i ++)
  {
    MB85RSx_Write((uint8_t*)buff,sector * MB85RSx_Para.SUBSECTOR_SIZE,MB85RSx_Para.SUBSECTOR_SIZE);
    sector ++;
    buff += MB85RSx_Para.SUBSECTOR_SIZE;
  }
  return RES_OK;
  /* USER CODE END WRITE */
}

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
DRESULT USER_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */
  DRESULT res = RES_ERROR;
  switch(cmd)
  {
    case CTRL_SYNC:
      res = RES_OK; 
      break;	 
    case GET_SECTOR_SIZE:
      *(WORD*)buff =  MB85RSx_Para.SUBSECTOR_SIZE;
      res = RES_OK;
      break;	 
    case GET_BLOCK_SIZE:
      *(DWORD*)buff =  MB85RSx_Para.SECTOR_SIZE;
      res = RES_OK;
      break;	 
    case GET_SECTOR_COUNT:
      *(DWORD*)buff = MB85RSx_Para.SUBSECTOR_COUNT;
      res = RES_OK;
      break;
    default:
      res = RES_PARERR;
      break;
  }
  return res;
  /* USER CODE END IOCTL */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
