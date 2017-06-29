/*
 * statusbar.h
 *
 *  Created on: 28.06.2017
 *      Author: David Paul
 */

#ifndef STATUSBAR_H_
#define STATUSBAR_H_

typedef struct{
	uint8_t title[60];
}StatusBarTypeDef;

void StatusbarShow(StatusBarTypeDef *StB);//StatusBarTypeDef *StBar);
void StatusbarSetTitle(StatusBarTypeDef *StB, uint8_t *Title);

#endif /* STATUSBAR_H_ */
