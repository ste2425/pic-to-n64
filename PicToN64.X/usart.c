/*
 Basic usart logger for pic 16f688
 */

#include <stdio.h>
#include "uart.h"
#include <pic16f688.h>

unsigned int lastInt;

void USART_init(void)
{
    TXSTAbits.TXEN = 1;     // enable transmitter
    TXSTAbits.BRGH = 1;     // high baud rate mode
    RCSTAbits.CREN = 1;     // enable continous receiving

    // configure I/O pins
    //ANSELbits.ANSB5 = 0;   // RX input type is digital
    TRISCbits.TRISC4 = 1;     // RX pin is input
    TRISCbits.TRISC5 = 1;     // TX pin is input (automatically configured)

    SPBRG = 51;//25;            // set baud rate to 9600 baud (4MHz/(16*baudrate))-1

    //PIE1bits.RCIE = 1;      // enable USART receive interrupt
    RCSTAbits.SPEN = 1;     // enable USART
}

/*
 Will send a single char
 */
void USART_putc(unsigned char c)
{
    while (!TXSTAbits.TRMT); // wait until transmit shift register is empty
    TXREG = c;               // write character to TXREG and start transmission
}

/*
 Will send a string
 */
void USART_puts(unsigned char *s)
{
    while (*s)
    {
        USART_putc(*s);     // send character pointed to by s
        s++;                // increase pointer location to the next character
    }
}

/*
 Will send a int
 */
void USART_puti(unsigned int i) {
    char str[15];
    sprintf(str, "%d\n", i);
    USART_puts(str);
}