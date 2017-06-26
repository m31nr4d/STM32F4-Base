/*
 * This file is part of the µOS++ distribution.
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

void InitClock();

int main(int argc, char* argv[])
{
	uint8_t linecount = 0;
	// At this stage the system clock should have already been configured
	// at high speed.
	HAL_Init();

	BSP_LED_Init(DISCO_LED1);
	BSP_LED_Toggle(DISCO_LED1);

	InitClock();

	BSP_LED_Init(DISCO_LED2);
	BSP_LED_Toggle(DISCO_LED2);

	BSP_LCD_InitEx(LCD_ORIENTATION_LANDSCAPE);

	BSP_LED_Init(DISCO_LED3);
	BSP_LED_Toggle(DISCO_LED3);

	BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_FB_START_ADDRESS);
	BSP_LCD_SetFont(&Font20);
	BSP_LCD_Clear(LCD_COLOR_BLACK);
	BSP_LCD_Clear(LCD_COLOR_WHITE);

	BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAtLine(linecount++, (uint8_t *) ">>SYS Init OK");
	BSP_LCD_DisplayStringAtLine(linecount++, (uint8_t *) ">>CLK Init OK");
	BSP_LCD_DisplayStringAtLine(linecount++, (uint8_t *) ">>RAM Init OK");
	BSP_LCD_DisplayStringAtLine(linecount++, (uint8_t *) ">>DIS Init OK");
	BSP_LCD_DisplayStringAtLine(linecount++, (uint8_t *) ">>SYS Init OK");

	uint8_t buf[10];
	uint8_t msg[] = ">>Boot Completed after [ms]: ";

	itoa(HAL_GetTick(), buf, 10);
	strcat(msg, buf);

	BSP_LCD_DisplayStringAtLine(linecount++, msg);


	BSP_LED_Init(DISCO_LED4);
	// Infinite loop
	while (1)
    {
		BSP_LED_Toggle(DISCO_LED4);
		HAL_Delay(100);
    }
}

void InitClock()
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

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
