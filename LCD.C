/*
 * LCD.c
 *
 */

#include <msp430.h>
#include "LCD.h"
#define RS_MASK 0x40

char LCDcon = 0;

void initSPI() { //Initializes the SPI subsytem.

	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 |= UCCKPH | UCMSB | UCMST | UCSYNC;
	UCB0CTL1 |= UCSSEL1;

	P1DIR |= BIT0;

	P1SEL |= BIT5;
	P1SEL2 |= BIT5;

	P1SEL |= BIT7;
	P1SEL2 |= BIT7;

	P1SEL |= BIT6;
	P1SEL2 |= BIT6;

	UCB0CTL1 &= ~UCSWRST;
}

void writeCommandNibble(char commandNibble);
void writeCommandByte(char commandByte);
void SPI_send(char byteToSend);
void delayMicro();

void LCDinit() {
	writeCommandNibble(0x03);
	writeCommandNibble(0x03);
	writeCommandNibble(0x03);
	writeCommandNibble(0x02);

	writeCommandByte(0x28);
	writeCommandByte(0x0C);
	writeCommandByte(0x01);
	writeCommandByte(0x06);
	writeCommandByte(0x01);
	writeCommandByte(0x02);

	SPI_send(0);
	delayMicro();
}

void LCDCLR() {
	writeCommandByte(1);
}

void LCD_write_8(char byteToSend);
void delayMilli();

void writeDataByte(char dataByte) {
	LCDcon |= RS_MASK;
	LCD_write_8(dataByte);
	delayMilli();
}

void writeString(char * string) {
	int i = 0;
	LCDcon |= RS_MASK;
	for (i = 0; i < 8; i++) {
		LCD_write_8(string[i]);
		delayMilli();
	}
}

void rotateString(char *string1, char *string2, int length) {
	unsigned int i = 0;

	char *count1 = string1, *count2 = string2;

	while (1) {
		updateCursor1();
		char *currentChar = count1;

		for (i = 0; i < 8; i++) {
			writeDataByte(*currentChar);

			currentChar++;

			if (*currentChar == 0)
				currentChar = string1;
		}
		count1++;

		if (*count1 == 0) {
			count1 = string1;
		}

		updateCursor2();
		char *currentChar2 = count2;
		for (i = 0; i < 8; i++) {
			writeDataByte(*currentChar2);

			currentChar2++;

			if (*currentChar2 == 0)
				currentChar2 = string2;
		}
		count2++;

		if (*count2 == 0) {
			count2 = string2;
		}

		__delay_cycles(665544);

		LCDCLR();
	}
}

void SET_SS_HI() {

	P1OUT |= BIT0;
}

void SET_SS_LO() {

	P1OUT &= ~BIT0;
}

void delayMicro() {

	__delay_cycles(0x09);
}

void delayMilli() {

	__delay_cycles(0x025F);
}

void LCD_write_4(char sendByte);

void writeCommandNibble(char commandNibble) {
	LCDcon &= ~RS_MASK;
	LCD_write_4(commandNibble);
	delayMilli();
}

void writeCommandByte(char commandByte) {
	LCDcon &= ~RS_MASK;
	LCD_write_8(commandByte);
	delayMilli();
}

void LCD_write_4(char sendByte) {

	sendByte &= 0x0F;
	sendByte |= LCDcon;
	sendByte &= 0x7F;
	SPI_send(sendByte);
	delayMicro();
	sendByte |= 0x80;
	SPI_send(sendByte);
	delayMicro();
	sendByte &= 0x7F;
	SPI_send(sendByte);
	delayMicro();
}

void updateCursor1() {
	writeCommandByte(0x80);
}

void updateCursor2() {
	writeCommandByte(0xC0);
}



void LCD_write_8(char byteToSend) {
	unsigned char sendByte = byteToSend;

	sendByte &= 0xF0;

	sendByte = sendByte >> 4;               // >> = rotate right

	LCD_write_4(sendByte);

	sendByte = byteToSend;

	sendByte &= 0x0F;

	LCD_write_4(sendByte);
}

void SPI_send(char byteToSend) {
	volatile char readByte;

	SET_SS_LO();

	UCB0TXBUF = byteToSend;

	while (!(UCB0RXIFG && IFG2)) {

	}

	readByte = UCB0RXBUF;

	SET_SS_HI();
}

int btnPressed() {

	int btnPress = 0;
	while (btnPress == 0) {
		if ((P1IN & BIT1)== 0){
			btnPress = 1;
			return btnPress;
		}

		if ((P1IN & BIT2)== 0){
			btnPress = 2;
			return btnPress;
		}

		if ((P1IN & BIT3)== 0){
			btnPress = 3;
			return btnPress;
		}
	}
	return btnPress;
}

void doWork(){
    char * requiredMsg = "ECE382 is my favorite class! "; // minus the GR's
    char * prompt = "Message?";
    char * promptKey = "Press123";

    char * message1 = "Brave Gryffindor... "; // expecto patronum
    int length1 = 23;

    char * message2 = "Sneaky Slytherin... ";
    int length2 = 17;

    char * message3 = "Nerdy Ravenclaw... ";
    int length3 = 17;

    updateCursor1();
    writeString(prompt);


    updateCursor2();
    writeString(promptKey);

    int buttonPushed = 0;

    buttonPushed = btnPressed();


    if (buttonPushed == 1) {
            rotateString(requiredMsg, message1, length1);
    }

    if (buttonPushed == 2) {
            rotateString(requiredMsg, message2, length2);
    }

    if (buttonPushed == 3) {
            rotateString(requiredMsg, message3, length3);
    }
}
