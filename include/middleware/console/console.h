//########################
// CONSOLE
//########################

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdio.h>

typedef enum{
	Inactive = 0x00,
	Display = 0x01,
	Trace = 0x02
}DbgConsoleChannel;


typedef struct{
	DbgConsoleChannel channel;
	uint8_t display_buffer[66][68];
	uint8_t buffer_Index;
}DbgConsoleTypeDef;

void ConsoleInit(DbgConsoleTypeDef *Dbg);
void ConsolePrint(DbgConsoleTypeDef *Dbg, uint8_t *Msg);

#endif /* CONSOLE_H_ */
