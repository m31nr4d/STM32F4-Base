/*
 * console.c
 *
 *  Created on: 14.06.2017
 *      Author: David Paul
 */

#include <drivers/lcd.h>
#include <middleware/console/console.h>


void ConsoleInit(DbgConsoleTypeDef *Dbg)
{
	if(Dbg->channel & Display)
	{
		BSP_LCD_InitEx(LCD_ORIENTATION_PORTRAIT);
		BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_FB_START_ADDRESS);
		BSP_LCD_SetFont(&Font12);
		BSP_LCD_Clear(0xFF2F2F2F);
		BSP_LCD_SetBackColor(0xFF2F2F2F);
		BSP_LCD_SetTextColor(0xFFFFFFFF);

		Dbg->buffer_Index = 0;

		for(uint8_t i = 0; i < NumLines; i++)
		{
			for(uint8_t j = 0; j < NumColumns; j++)
			{
				Dbg->display_buffer[i][j].character = ' ';

			}
		}
	}
}



void ConsolePrint(DbgConsoleTypeDef *Dbg, uint8_t *Msg)
{
	uint8_t CurLength = 0;

	while(*Msg)
	{
		if(*Msg == '\n' || *Msg == '\r' || CurLength == NumColumns)
		{
			if(CurLength != 0)
			{
				Dbg->buffer_Index++;
				if(Dbg->buffer_Index == NumLines)
				{
					Dbg->buffer_Index = 0;
				}
				for(uint8_t i = 0; i < NumColumns; i++)
				{
					Dbg->display_buffer[Dbg->buffer_Index][i].character = ' ';
				}
			}
		}
		else
		{
			Dbg->display_buffer[Dbg->buffer_Index][CurLength].character = *Msg;
			CurLength++;
		}

		if(Dbg->buffer_Index == NumLines)
		{
			Dbg->buffer_Index = 0;
		}
		Msg++;
	}

	while(CurLength != NumColumns)
	{
		Dbg->display_buffer[Dbg->buffer_Index][CurLength].character = ' ';
		CurLength++;
	}

	ConsoleShow(Dbg);

	return;
}

void ConsoleShow(DbgConsoleTypeDef *Dbg)
{
	int8_t line_Index = Dbg->buffer_Index;
	for(int8_t i = NumLines; i > 0; i--)
	{
		for(uint8_t j = 0; j < NumColumns; j++)
		{
			if(i == NumLines)
			{
				BSP_LCD_SetBackColor(0xFF008000);
				BSP_LCD_SetTextColor(0xFF404040);
			}
			else
			{
				BSP_LCD_SetBackColor(0xFF2F2F2F);
				BSP_LCD_SetTextColor(0xFFFFFFFF);//Dbg->display_buffer[line_Index][j].format);
			}
			BSP_LCD_DisplayChar(j * 7, (i-1) * 12, Dbg->display_buffer[line_Index][j].character);
		}
		line_Index --;
		if(line_Index < 0)
			line_Index = NumLines-1;
	}
}
