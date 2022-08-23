/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
#if (USE_MB85RS4M == 1)
  #include "mb85rsxx.h"
#endif
#if (USE_W25QXX == 1)
  #include "w25qxx.h"
#endif
#include <stdio.h>
#include <string.h>
#include "rtc.h"
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
uint8_t workbuf[4096];
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  
  if(retUSER == FR_OK)
  {
    retUSER = f_mount(&USERFatFS,(TCHAR const*)USERPath,1) ;
    if(retUSER != FR_OK)
    {
      retUSER = 2;
      if(!f_mkfs((TCHAR const*)USERPath, (FM_SFD | FM_FAT), 0,workbuf,4096))
      {
        retUSER = FR_OK;
      }
    }
  }
  //f_mkfs((TCHAR const*)USERPath, (FM_SFD | FM_FAT), 0,workbuf,4096);
    
  /* USER CODE END Init */
}


/**
  * @brief  Create Init File
  * @param  None (void)
  * @retval None (void)
  */
void Create_FATFS_Init_File(void)
{
  
  char wtext[100];
  uint32_t byteswritten;                     /* File write/read counts */
  
  if(retUSER == FR_OK)
  {
    retUSER = f_open(&USERFile, "WeAct Core-Board Fram Flash.txt", FA_CREATE_ALWAYS | FA_WRITE);
    //(retUSER == FR_EXIST) Don't re
    if(retUSER == FR_OK )
    {
#if (USE_MB85RS4M == 1)
      //TODO Para stuct can be a common type
      sprintf(wtext,"MB85RS4M\r\nFlash ID: 0x%X\r\nFlash Size: %d kB\r\n\r\n",MB85RSx_Para.FLASH_ID,MB85RSx_Para.FLASH_SIZE / 1024 );
#endif
      retUSER = f_write(&USERFile, wtext, strlen(wtext), (void *)&byteswritten);

      sprintf(wtext,"This is STM32 working with FatFs\r\nSTM32F411 Core Board Design By WeAct\r\nBoard Version: 3.1\r\n");
      retUSER |= f_write(&USERFile, wtext, strlen(wtext), (void *)&byteswritten);

      f_close(&USERFile);
    }
  }
}


/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
	uint16_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;	
  /* 必须同时获取时间和日期 不然会导致下次RTC不能读取 */
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);	
	
	year = sdatestructureget.Year + 2000;
	month = sdatestructureget.Month;
	date = sdatestructureget.Date;
	hours = stimestructureget.Hours;
	minutes = stimestructureget.Minutes;
	seconds = stimestructureget.Seconds;
	
	return ((year - 1980) << 25) /* Year */
	| (month << 21) /* Month */
	| (date << 16) /* Day */
	| (hours << 11) /* Hour */
	| (minutes << 5) /* Min */
	| (seconds) /* Sec */
	;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
