/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

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
#include "middleware/fat/ff_gen_drv.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
           
DSTATUS QSPI_initialize (BYTE pdrv);
DSTATUS QSPI_status (BYTE pdrv);
DRESULT QSPI_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT QSPI_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT QSPI_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  QSPI_Driver =
{
  QSPI_initialize,
  QSPI_status,
  QSPI_read,
#if  _USE_WRITE
  QSPI_write,
#endif  /* _USE_WRITE == 1 */  
#if  _USE_IOCTL == 1
  QSPI_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS QSPI_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
	Stat = STA_NOINIT;
	/* Configure the QSPI device */
	if(BSP_QSPI_Init() == QSPI_OK)
	{
	    Stat &= ~STA_NOINIT;
	}
    return Stat;
}

/**
  * @brief  Gets Disk Status 
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS QSPI_status (
	BYTE pdrv       /* Physical drive nmuber to identify the drive */
)
{
	  Stat = STA_NOINIT;

	  if(BSP_QSPI_GetStatus() == QSPI_OK)
	  {
	    Stat &= ~STA_NOINIT;
	  }

	  return Stat;
}

/**
  * @brief  Reads Sector(s) 
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT QSPI_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
	  DRESULT res = RES_ERROR;
	  uint32_t timeout = 100000;

	  if(BSP_QSPI_Read((uint32_t*)buff,
	                       (uint32_t) (sector * _MAX_SS),
	                       count *  _MAX_SS) == QSPI_OK)
	  {
	    while(BSP_QSPI_GetStatus()!= QSPI_OK)
	    {
	      if (timeout-- == 0)
	      {
	        return RES_ERROR;
	      }
	    }
	    res = RES_OK;
	  }

	  return res;
}

/**
  * @brief  Writes Sector(s)  
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT QSPI_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{ 
	  DRESULT res = RES_ERROR;
	  uint32_t timeout = 100000;

	  if(BSP_QSPI_Write( (uint32_t *) buff,
	                       (uint32_t) (sector * _MAX_SS),
	                       count *  _MAX_SS) == QSPI_OK)
	  {
	    while(BSP_QSPI_GetStatus()!= QSPI_OK)
	    {
	      if (timeout-- == 0)
	      {
	        return RES_ERROR;
	      }
	    }
	    res = RES_OK;
	  }

	  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation  
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT QSPI_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
	  DRESULT res = RES_ERROR;
	  QSPI_InfoTypeDef CardInfo;

	  switch (cmd)
	  {
	  /* Make sure that no pending write process */
	  case CTRL_SYNC :
	    res = RES_OK;
	    break;

	  /* Get number of sectors on the disk (DWORD) */
	  case GET_SECTOR_COUNT :
	    BSP_QSPI_GetInfo(&CardInfo);
	    *(DWORD*)buff = 0x10000 / 2;
	    res = RES_OK;
	    break;

	  /* Get R/W sector size (WORD) */
	  case GET_SECTOR_SIZE :
	    BSP_QSPI_GetInfo(&CardInfo);
	    *(WORD*)buff = 0x100 * 2;
	    res = RES_OK;
	    break;

	  /* Get erase block size in unit of sector (DWORD) */
	  case GET_BLOCK_SIZE :
	    BSP_QSPI_GetInfo(&CardInfo);
	    *(DWORD*)buff = 4096;
	    res = RES_OK;
	    break;

	  default:
	    res = RES_PARERR;
	  }

	  return res;
}
#endif /* _USE_IOCTL == 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
