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


// Erzeugt im grünen Konsolenbereich einen Bargraph; val [0.0 ... 1.0]
// der Bargraph wird von einer evtl. anderen Debugausgabe überschrieben. Zum speichern eine (abgeschlossenen) bargraph mit save = 1 aufrufen!
// dadurch wird der Bargraph mittels \n übernommen

void ConsolePrintBargraph(DbgConsoleTypeDef *Dbg, float val, uint8_t save)
{
	if( val < 0.0)
		val = 0.0;
	if(val > 1.0)
		val = 1.0;

	uint8_t progress = val * (NumColumns -15.0);
	uint8_t blank = NumColumns - 15 - progress;
	uint8_t BufProgress[NumColumns] = {0};
	uint8_t BufBlank[NumColumns] = {0};
	uint8_t BufPrcnt[10] = {0};
	uint8_t Buffer[NumColumns] = "|";


	memset(BufProgress, '>', progress );
	memset(BufBlank, '-', blank );
	sprintf(BufPrcnt, "| [%d %%]", (int) (val*100.0));
	strcat(Buffer, BufProgress);
	strcat(Buffer, BufBlank);
	strcat(Buffer, BufPrcnt);
	if(save)
		strcat(Buffer, "\n");
	ConsolePrint(Dbg, Buffer);
}


// Fügt der Konsole zeichen Hinzu; Wenn diese nicht mit \n in eine neue Zeile geladen werden, wird die Eingabe beim nächsten Schreiben überschrieben
// ( -> grüner Konsolenbereich)

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

// Rendert die Konsole in ein Fenster (untere 300px des Displays)

void ConsoleShow(DbgConsoleTypeDef *Dbg)
{
	int8_t line_Index = Dbg->buffer_Index;
	for(int8_t i = NumLines; i > 0; i--)
	{
		for(uint8_t j = 0; j < NumColumns; j++)
		{
			if(i == NumLines)
			{
				BSP_LCD_SetBackColor(0xFF00DF00);
				BSP_LCD_SetTextColor(0xFF000000);
			}
			else
			{
				BSP_LCD_SetBackColor(0xFF2F2F2F);
				BSP_LCD_SetTextColor(0xFFFFFFFF);//Dbg->display_buffer[line_Index][j].format);
			}
			BSP_LCD_DisplayChar((j * 7) + 1, ((i-1) * 12) + 523, Dbg->display_buffer[line_Index][j].character);
		}
		line_Index --;
		if(line_Index < 0)
			line_Index = NumLines-1;
	}

	BSP_LCD_SetTextColor(0xFFFFFFFF);
	BSP_LCD_DisplayStringAt(10, 506, "Debug Console:", LEFT_MODE);
	BSP_LCD_DrawRect(0,500,479,22);
	BSP_LCD_DrawRect(0,500,479,299);
}
