//########################
// CONSOLE
//########################

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdio.h>

#define NumColumns 68
#define NumLines 66


typedef enum{
	Inactive = 0x00,
	Display = 0x01,
	Trace = 0x02
}DbgConsoleChannel;

typedef struct{
	uint8_t character;
	uint8_t format;
}DbgConsoleCharTypeDef;

typedef struct{
	DbgConsoleChannel channel;
	DbgConsoleCharTypeDef display_buffer[NumLines][NumColumns];
	uint8_t buffer_Index;
}DbgConsoleTypeDef;

void ConsoleInit(DbgConsoleTypeDef *Dbg);
void ConsolePrint(DbgConsoleTypeDef *Dbg, uint8_t *Msg);
void ConsoleShow(DbgConsoleTypeDef *Dbg);

#endif /* CONSOLE_H_ */
