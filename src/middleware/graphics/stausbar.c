/*
 * stausbar.c
 *
 *  Created on: 28.06.2017
 *      Author: David Paul
 */
#include <drivers/lcd.h>
#include "middleware/graphics/statusbar.h"

void StatusbarShow()//StatusBarTypeDef *StBar)
{
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_BACKGROUND);
	BSP_LCD_SetBackColor(0xFF2F2F2F);
	BSP_LCD_SetTextColor(0xFFFFFFFF);
	BSP_LCD_DrawRect(0,0,479,20);
	BSP_LCD_DisplayStringAt(10, 6, "Bat: 34%         running micropython.py      WiFi: OFF | Sat: OFF", LEFT_MODE);

	BSP_LCD_SetTextColor(0xFFFFFFFF);
	BSP_LCD_FillRect(0,21,480,479);
}
