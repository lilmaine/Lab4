#include <msp430.h>
#include "LCD.h"

/*
 * main.c
 */
int main(void) {
        WDTCTL = WDTPW | WDTHOLD;        // Stop watchdog timer

        initSPI(); // same as asm lab
        LCDinit();
        LCDCLR();
        doWork(); // put the bulk of the main work in lcd.c to keep main.c concise

        return 0;
}


