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
#include "bitmap_db.h"

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
	ConsolePrint(&Dbg, (uint8_t *) "Graphics Demo:\n");

	BSP_LCD_SetTextColor(0xFFFFFFFF);
	BSP_LCD_SetFont(&Font24);


	//++++++++++++++++++++++++++++++++++++++ Keyboard +++++++++++++++++++++++++++++++++++

	/*uint16_t cornersx[34] = {0};
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
	}*/

//++++++++++++++++++++++++++++++++++++++++++++++ New Fonts ++++++++++++++++++++++++++++++

	/*FONT_INFO* pFont = &arial_10ptFontInfo;
	DrawNewChar(10,100,pFont);
	pFont = &arial_10BptFontInfo;
	DrawNewChar(10,120,pFont);
	pFont = &arial_18ptFontInfo;
	DrawNewChar(10,140,pFont);
	pFont = &arial_18BptFontInfo;
	DrawNewChar(10,160,pFont);
	pFont = &arial_24ptFontInfo;
	DrawNewChar(10,200,pFont);
	pFont = &arial_24BptFontInfo;
	DrawNewChar(10,250,pFont);*/

//+++++++++++++++++++++++++++++++++++++++++++ Bargraph ++++++++++++++++++++++++++++++++++++++++

	uint8_t numBars = 9;
	uint8_t val[9] = {24,234,100,45,179,255,90,105,234};

	typedef struct{
		uint32_t color;
		uint8_t pattern;
	}style;

	uint32_t colors[] = {
			LCD_COLOR_BLUE,
			LCD_COLOR_GREEN,
			LCD_COLOR_RED,
			LCD_COLOR_CYAN,
			LCD_COLOR_MAGENTA,
			LCD_COLOR_YELLOW,
			LCD_COLOR_ORANGE,
			LCD_COLOR_LIGHTBLUE,
			LCD_COLOR_LIGHTGREEN,
			LCD_COLOR_LIGHTRED,
			LCD_COLOR_LIGHTCYAN,
			LCD_COLOR_LIGHTMAGENTA,
			LCD_COLOR_LIGHTYELLOW,
			LCD_COLOR_DARKBLUE,
			LCD_COLOR_DARKGREEN,
			LCD_COLOR_DARKRED,
			LCD_COLOR_DARKCYAN,
			LCD_COLOR_DARKMAGENTA,
			LCD_COLOR_DARKYELLOW
	};

	Point origin = {40,40};
	Point size = {400,300};

	uint8_t barsize = (size.X - 20) / (numBars + 1);
	uint8_t gapsize = barsize / numBars;



	BSP_LCD_DrawLine(origin.X, origin.Y, origin.X, origin.Y + size.Y);
	BSP_LCD_DrawLine(origin.X, origin.Y + size.Y,  origin.X + size.X,  origin.Y + size.Y);

	for(uint8_t i = 0; i < numBars; i++)
	{
		BSP_LCD_SetTextColor(colors[i]);
		BSP_LCD_FillRectPattern(origin.X + i*barsize + (i+1)*gapsize, origin.Y + size.Y- size.Y * ((float)val[i]/255.0) - 1, barsize, size.Y * ((float)val[i]/255.0));
	}

	while (1)
    {

    }
}

void DrawNewChar(uint16_t Xpos, uint16_t Ypos, FONT_INFO* pFont)
{
  uint32_t i = 0, j = 0;
  uint64_t line;
  uint8_t* pchar;



uint8_t* pch = &(pFont->data[pFont->charInfo['A' - pFont->startChar].offset]);
uint8_t width = pFont->charInfo['A' - pFont->startChar].widthBits;
uint8_t height = pFont->charInfo['A' - pFont->startChar].heightBits;
uint8_t offset =  8 *((width + 7)/8) -  width ;

  for(i = 0; i < height; i++)
  {
    pchar = ((uint8_t*)pch + (width + 7)/8 * i);

    switch(((width + 7)/8))
    {

    case 1:
      line =  pchar[0];
      break;

    case 2:
      line =  (pchar[0]<< 8) | pchar[1];
      break;

    case 3:
      line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
      break;

    case 4:
      line =  (pchar[0]<< 24) | (pchar[1]<< 16) | (pchar[2]<< 8) | pchar[3];
      break;

    case 5:
      line =  (pchar[0]<< 32) | (pchar[1]<< 24) | (pchar[2]<< 16) | (pchar[3]<< 8) | pchar[4];
      break;

    case 6:
      line =  (pchar[0]<< 40) | (pchar[1]<< 32) | (pchar[2]<< 24) | (pchar[3]<< 16) | (pchar[4]<< 8) | pchar[5];
      break;

    case 7:
      line =  (pchar[0]<< 48) | (pchar[1]<< 40) |(pchar[2]<< 32) | (pchar[3]<< 24) | (pchar[4]<< 16) | (pchar[5]<< 8) | pchar[6];
      break;

    case 8:
      line =  (pchar[0]<< 56) | (pchar[1]<< 48) |(pchar[2]<< 40) |(pchar[3]<< 32) | (pchar[4]<< 24) | (pchar[5]<< 16) | (pchar[6]<< 8) | pchar[7];
      break;

    default:
      line = 0b1010101010101010101010101010101010101010101010101010101010101010;
      break;
    }

    for (j = 0; j < width; j++)
    {
      if(line & (1 << (width- j + offset- 1)))
      {
        BSP_LCD_DrawPixel((Xpos + j), Ypos, 0xFFFFFFFF);
      }
      else
      {
        BSP_LCD_DrawPixel((Xpos + j), Ypos,0xFF000000);
      }
    }
    Ypos++;
  }
}



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
