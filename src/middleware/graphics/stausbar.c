/*
 * stausbar.c
 *
 *  Created on: 28.06.2017
 *      Author: David Paul
 */
#include <drivers/lcd.h>
#include "middleware/graphics/statusbar.h"

void StatusbarShow(StatusBarTypeDef *StB)//StatusBarTypeDef *StBar)
{
	BSP_LCD_SetFont(&Font12);
	BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_BACKGROUND);
	BSP_LCD_SetBackColor(0xFF2F2F2F);
	BSP_LCD_SetTextColor(0xFFFFFFFF);
	BSP_LCD_DrawRect(0,0,479,20);
	BSP_LCD_DisplayStringAt(10, 6, StB->title, LEFT_MODE);
}

void StatusbarSetTitle(StatusBarTypeDef *StB, uint8_t *Title)
{
	strcpy(StB->title, Title);
	for(uint8_t i = strlen(Title); i < 60; i++)
		strcat(StB->title, " ");
	StatusbarShow(StB);
}
