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
		BSP_LCD_InitEx(LCD_ORIENTATION_LANDSCAPE);
		BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_FB_START_ADDRESS);
		BSP_LCD_SetFont(&Font20);
		BSP_LCD_Clear(0xFF2F2F2F);
		BSP_LCD_SetBackColor(0xFF2F2F2F);
		BSP_LCD_SetTextColor(0xFFFFFFFF);

		Dbg->buffer_Index = 0;

		for(uint8_t i = 0; i < 25; i++)
		{
			for(uint8_t j = 0; j < 58; j++)
			{
				Dbg->display_buffer[i][j] = 0;

			}
		}
	}
}

// TODO
// Get the Buffer right; somehow ConsolePrint skips an input; somehow it prints a line below the lower display border

void ConsolePrint(DbgConsoleTypeDef *Dbg, uint8_t *Msg)
{
	if(Dbg->channel & Display)
	{
		char text[57] = {0};
		strcpy(text, Msg);
		if(Dbg->buffer_Index == 24)
		{
			Dbg->buffer_Index = 0;
		}

		strcpy(Dbg->display_buffer[Dbg->buffer_Index], (uint8_t *) text);
		memset(text , ' ', 57 - strlen(text));
		strcat(Dbg->display_buffer[Dbg->buffer_Index], (uint8_t *) text);
		int8_t msg_index = Dbg->buffer_Index;

		for(uint8_t i = 24; i > 0; i--)
		{
			BSP_LCD_DisplayStringAtLine(i-1, Dbg->display_buffer[msg_index]);
			msg_index--;
			if(msg_index < 0)
				msg_index = 24;
		}
		Dbg->buffer_Index++;
	}
	if(Dbg->channel & Trace)
	{
		trace_printf(Msg);
		trace_printf("\n");
	}

}
