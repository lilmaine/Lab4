/*
 * LCD.h
 *
 *  Created on: Oct 21, 2013
 *      Author: C2C Tramaine Barnett
 *      Description: This library will act as a driver to interact with the LCD on the Geek Box.
 */

#ifndef LCD_H_
#define LCD_H_

void initSPI();

void LCDinit();

void LCDCLR();

void updateCursor1();

void updateCursor2();

int btnPressed();

void writeDatatByte(char dataByte);

void writeString(char * string);

void rotateString(char * string1, char * string2, int length);

void doWork();

#endif /* LCD_H_ */
