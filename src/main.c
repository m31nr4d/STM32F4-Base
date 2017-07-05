/*
 * This file is part of the ÂµOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include "global.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//########################
// HW CONFIG
//########################
void ClockInit()
{
	RCC_OscInitTypeDef OscConfig;
	HAL_RCC_GetOscConfig(&OscConfig);
	// Enable HSE Oscillator and activate PLL with HSE as source.
	// This is tuned for STM32F4-DISCOVERY; update it for your board.
	OscConfig.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	OscConfig.HSEState = RCC_HSE_ON;
	OscConfig.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	// This assumes the HSE_VALUE is a multiple of 1 MHz. If this is not
	// your case, you have to recompute these PLL constants.
	OscConfig.PLL.PLLM = (HSE_VALUE/1000000u);
	OscConfig.PLL.PLLN = 360;
	OscConfig.PLL.PLLP = RCC_PLLP_DIV2;
	OscConfig.PLL.PLLQ = 7; /* To make USB work. */
	OscConfig.PLL.PLLR = 2;
	OscConfig.PLL.PLLState = RCC_PLL_ON;
	HAL_RCC_OscConfig(&OscConfig);

	RCC_ClkInitTypeDef ClkCconfig;
	// Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	// clocks dividers
	ClkCconfig.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
	      | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	ClkCconfig.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	// This is expected to work for most large cores.
	// Check and update it for your own configuration.
	ClkCconfig.AHBCLKDivider = RCC_SYSCLK_DIV1;
	ClkCconfig.APB1CLKDivider = RCC_HCLK_DIV4;
	ClkCconfig.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&ClkCconfig, FLASH_LATENCY_5);
}

void HWInit()
{
	HAL_Init();
	ClockInit();
	BSP_LED_Init(DISCO_LED4);
	BSP_LED_Init(DISCO_LED3);
	BSP_LED_Init(DISCO_LED2);
	BSP_LED_Init(DISCO_LED1);

	BSP_LCD_InitEx(LCD_ORIENTATION_PORTRAIT);
	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_FB_START_ADDRESS);
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_Clear(0xFF2F2F2F);


}

//########################
// MAIN
//########################

FATFS SdFs, QspiFs;		/* FatFs work area needed for each volume */
FIL Fil, Fil2;			/* File object needed for each open file */
DbgConsoleTypeDef Dbg; // Console Object
StatusBarTypeDef Stb;
SD_HandleTypeDef hsd;
TS_StateTypeDef sts;


int main(int argc, char* argv[])
{

	hsd.Instance = SDIO;
	hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
	hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	hsd.Init.ClockDiv = 0;

	Dbg.channel = Display; // | Trace;


	HWInit();

	ConsoleInit(&Dbg);
	StatusbarSetTitle(&Stb, (uint8_t *) "Welcome!");

	ConsolePrint(&Dbg, (uint8_t *) "#########################\n");
	ConsolePrint(&Dbg, (uint8_t *) "####  W E L C O M E  ####\n");
	ConsolePrint(&Dbg, (uint8_t *) "#### (c) David Paul  ####\n");
	ConsolePrint(&Dbg, (uint8_t *) "#### Version: 0.0.2  ####\n");
	ConsolePrint(&Dbg, (uint8_t *) "#########################\n");
	ConsolePrint(&Dbg, (uint8_t *) " \n");
	ConsolePrint(&Dbg, (uint8_t *) "System Init OK\n");
	ConsolePrint(&Dbg, (uint8_t *) "Clock Init OK\n");
	ConsolePrint(&Dbg, (uint8_t *) "SDRAM Init OK\n");
	ConsolePrint(&Dbg, (uint8_t *) "Debug Init OK\n");

	if(BSP_TS_Init(480,800)  == TS_OK)
	{
		ConsolePrint(&Dbg, "Touch Init OK\n");
	}
	else
	{
		ConsolePrint(&Dbg, "Touch Init FAIL\n");
	}

	FATFS_Init();

	ConsolePrint(&Dbg, (uint8_t *) "FAT Fs Init Ok\n");

	BSP_LED_Toggle(DISCO_LED1);
	FRESULT res = f_mount(&SdFs, "SD:/", 1);
	if(res == RES_OK)
	{
		ConsolePrint(&Dbg, (uint8_t *) "> SD Card Mounted as 'SD:/'\n");
		BSP_LED_Toggle(DISCO_LED1);
	}
	else
	{
		uint8_t buf[57] = {0};
		sprintf(buf, "> SD Card Not Mounted (Err: %d )\n", res);
		ConsolePrint(&Dbg, buf);
	}


	BSP_LED_Toggle(DISCO_LED2);
	res = f_mount(&QspiFs, "QSPI:/", 1);
	if(res == RES_OK)
	{
		ConsolePrint(&Dbg, (uint8_t *) "> NOR Flash Mounted as 'QSPI:/'\n");
		BSP_LED_Toggle(DISCO_LED2);
	}
	else
	{
		uint8_t buf[57] = {0};
		sprintf(buf,(uint8_t *) "> NOR Flash Not Mounted (Err: %d )\n", res);
		ConsolePrint(&Dbg, buf);
	}


	ConsolePrint(&Dbg, (uint8_t *) " \n");
	ConsolePrint(&Dbg, (uint8_t *) "Touchsreen Demo:\n");

	BSP_LCD_SetTextColor(0xFFFFFFFF);
	BSP_LCD_SetFont(&Font24);

	uint16_t cornersx[34] = {0};
	uint16_t sizex[34] = {0};
	uint16_t cornersy[34] = {0};
	uint16_t sizey[34] = {0};
	uint8_t keymap[3][34] = {{'Q','W','E','R','T','Z','U','I','O','P','Ü','A','S','D','F','G','H','J','K','L','Ö','Ä','Y','X','C','V','B','N','M','^','<','1','~'},
			{'q','w','e','r','t','z','u','i','o','p','ü','a','s','d','f','g','h','j','k','l','ö','ä','y','x','c','v','b','n','m','^','<','1','~'},
			{'1','2','3','4','5','6','7','8','9','0','+','_','<','=','>','#','{','[',']','}','*','-',':',';','%','$','&','~','.','^','<','1','~'}};

	for(uint8_t i = 0; i < 11 ; i++)
	{
			//BSP_LCD_DrawRect(i*39 + 6, 21,35, 50); // First Row Of Chars
			//BSP_LCD_DrawRect(i*39 + 6, 81,35, 50); // Second Row

		// First Row Chars
		cornersx[i] = i*43 + 3;
		cornersy[i] = 21;
		sizex[i] = 37;
		sizey[i] = 50;
		// 2nd Row Chars
		cornersx[i+11] = i*43 + 3;
		cornersy[i+11] = 81;
		sizex[i+11] = 37;
		sizey[i+11] = 50;
		// 3rd Row Chars
		if( i < 8)
		{
			cornersx[i+21] = (i+1)*43 + 3;
			cornersy[i+21] = 141;
			sizex[i+21] = 37;
			sizey[i+21] = 50;
		}
	}

	// Shift
	cornersx[29] = 3;
	cornersy[29] = 141;
	sizex[29] = 80;
	sizey[29] = 50;
	// Backspace
	cornersx[30] = 9*43 + 3;
	cornersy[30] = 141;
	sizex[30] = 80;
	sizey[30] = 50;
	// Numbers
	cornersx[31] = 3;
	cornersy[31] = 201;
	sizex[31] = 80;
	sizey[31] = 50;
	// Space
	cornersx[32] = 2*43 + 3;
	cornersy[32] = 201;
	sizex[32] = 7 * 43 - 5;
	sizey[32] = 50;
	// Enter
	cornersx[33] = 9*43 + 3;
	cornersy[33] = 201;
	sizex[33] = 80;
	sizey[33] = 50;

	for(uint8_t i = 0; i < 34; i++)
	{
		BSP_LCD_DrawRect(cornersx[i], cornersy[i], sizex[i], sizey[i]);
		BSP_LCD_DisplayChar(cornersx[i]+2, cornersy[i]+2, keymap[2][i]);
	}

	/*for(uint8_t i = 2; i < 10 ; i++)
	{
		BSP_LCD_DrawRect(i*39 + 6, 141 ,35, 50); // Third Row
	}*/


	/*BSP_LCD_DrawRect(6, 201 , 72, 50);	// Nbrs (3|0-1)
	BSP_LCD_DrawRect(2*39 + 6, 201 ,8 * 39 - 6, 50); // Space (3|2-9)
	BSP_LCD_DrawRect(10*39 + 6, 201 ,72, 50); // Enter (3|10-11)*/

	while (1)
    {
		//BSP_LED_Toggle(DISCO_LED4);
		//HAL_Delay(5);
		//BSP_TS_GetState(&sts);
		/*if(sts.touchDetected > 0)
		{
			BSP_LED_On(DISCO_LED3);
			uint8_t buf[60] = {0};
			sprintf(buf, "Touch Detected! [%d|%d]", sts.touchX[0], sts.touchY[0]);
			StatusbarSetTitle(&Stb, buf);
			if((sts.touchY[0] < 500) && (sts.touchY[0] > 20))
			{
				uint8_t xTile = (sts.touchX[0]/(480/6));
				uint8_t yTile = ((sts.touchY[0]-21)/(480/6));
				BSP_LCD_SetTextColor(0xFF00FF00);
				BSP_LCD_FillRect(xTile*(480/6),yTile*(480/6) + 21,(480/6),(480/6));
				uint8_t buf[57] = {0};
				sprintf(buf, "> Tile #(%d|%d) pressed!\n", xTile,yTile);
				ConsolePrint(&Dbg, buf);
			}
		}
		else
		{
			BSP_LED_Off(DISCO_LED3);
			StatusbarSetTitle(&Stb, (uint8_t *) "Touch the Display!");
			BSP_LCD_SetTextColor(0xFFFF0000);
			BSP_LCD_FillRect(0,21,479,479);
		}

		BSP_LCD_SetTextColor(0xFF000000);
		for(uint8_t i = 0; i < 6 ; i++)
		{
			for(uint8_t j = 0; j < 6 ; j++)
			{
				BSP_LCD_DrawRect(i*(480/6),j*(480/6) + 21,(479/6),(479/6));
			}
		}*/
    }
}



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
