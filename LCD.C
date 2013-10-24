/*
 * LCD.C
 *
 *  Created on: Oct 22, 2013
 *      Author: C15Tramaine.Barnett
 */
#include <msp430g2553.h>
#include "lcd.h"
#define RS_MASK 0x40
static int LCDCON;
static int LCDSEND;

void writeCommandNibble(char commandNibble);
void writeCommandByte(char commandByte);
void SPI_send(char byteToSend);
void LCD_write_8(char byteToSend);
void LCD_write_4(char byteToSend);

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

void writeCommandNibble(char commandNibble) {
	LCDCON &= ~RS_MASK;
	LCD_write_4(commandNibble);
	delayMilli();
}

void writeCommandByte(char commandByte) {
	LCDCON &= ~RS_MASK;
	LCD_write_8(commandByte);
	delayMilli();
}

void writeDataByte(char dataByte) {
	LCDCON |= RS_MASK;
	LCD_write_8(dataByte);
	delayMilli();
}

void LCD_write_8(char byteToSend) {
	unsigned char sendByte = byteToSend;

	sendByte &= 0xF0;

	sendByte = sendByte >> 4;               // rotate to the right 4 times

	LCD_write_4(sendByte);

	sendByte = byteToSend;

	sendByte &= 0x0F;

	LCD_write_4(sendByte);
}

void SPI_send(char byteToSend) {
	char readByte;

	set_SS_lo();

	UCB0TXBUF = byteToSend;

	while (!(UCB0RXIFG & IFG2)) {
		// wait until you've received a byte
	}

	readByte = UCB0RXBUF;

	set_SS_hi();
}

void initSPI() {

	UCB0CTL1 |= UCSWRST | UCSSEL1;
	UCB0CTL0 |= UCCKPH | UCMSB | UCMST | UCSYNC;
	UCB0CTL1 |= UCSSEL1;                       //selects clock to use
	UCB0STAT |= UCLISTEN;                      //enables internal loopback

	P1DIR |= BIT0;                                                   //For my SS

	P1SEL |= BIT5;                            // make UCB0CLK available on P1.5
	P1SEL2 |= BIT5;

	P1SEL |= BIT7;                           // make UCB0SSIMO available on P1.7
	P1SEL2 |= BIT7;

	P1SEL |= BIT6;                           // make UCB0SSOMI available on P1.6
	P1SEL2 |= BIT6;

	UCB0CTL1 &= ~UCSWRST;                       //enable subsystem
}

void SET_SS_HI() {

	P1OUT |= BIT0;
}

void SET_SS_LO() {

	P1OUT &= BIT0;
}

void LCDclr() {
	writeCommandByte(1);
}

void LCD_write_4(char sendByte) {

	sendByte &= 0x0f;                      // ensure upper half of byte is clear
	sendByte |= LCDCON;                                 //set LCD control nibble
	sendByte &= 0x7f;                                               //set E low
	SPI_send(sendByte);
	delayMicro();
	sendByte |= 0x80;                                               //set E high
	SPI_send(sendByte);
	delayMicro();
	sendByte &= 0x7f;                                               //set E low
	SPI_send(sendByte);
	delayMicro();
}

void writeString(char * string) {
	LCDCON |= RS_MASK;
	int i;
	for (i = 0; i < 8 ; i++){
		LCD_write_8(string[i]); //call wrt8 bit by bit
		delayMilli();
	}

}

