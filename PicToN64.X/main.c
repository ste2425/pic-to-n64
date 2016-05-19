/*
 * File:   main.c
 * Author: SCooper
 *
 * Created on 14 May 2016, 18:41
 * PIC: 16f688
 * Compiler: XC8
 */
#include <xc.h>

#pragma config FOSC = INTOSCIO // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled). If timer not cleared will restart PIC.
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enable

#define _XTAL_FREQ 4000000

int main()
{
    int adcRes;

    TRISC = 0xFF; //Reading channel 4. Its on TRISC so mark Whole port as input. Sledgehammer

    ANSELbits.ANS4 = 1;     //Put channel 4 into analogue mode
    
    TRISCbits.TRISC5 = 0; //Mark RC5&4 as output for LED's
    TRISCbits.TRISC4 = 0;

    /* ADC SETUP */
    ADCON0bits.ADFM = 1;        //ADC result is right justified
    ADCON0bits.VCFG = 0;        //Mark VDD as +VE reference
    
    /*
     Set the conversion clock to Fosc/8
     * This is because the conversion clock period (Tad) must be greater than
     * 1.5us. With a clock of 4MHz, Fosc/8 = Tad = 2us
     */
    ADCON1bits.ADCS = 0b001;    
    
    ADCON0bits.CHS = 4;         //Select ADC channel
    ADCON0bits.ADON = 1;        //Turn it on

    while(1)
    {
        __delay_us(5);              //Wait the acquisition time. See datasheet for value

        ADCON0bits.GO = 1;          //start conversion
        while(ADCON0bits.GO==1){};  //Wait to complete

        adcRes = (ADRESH<<8)+ADRESL;	//combine the 10 bits of the conversion. PIC16f688 is 8 so two separate registers make up value.

        if(adcRes > 512)
            PORTCbits.RC5 = 1;      
        else
            PORTCbits.RC5 = 0;     
    }

    return 0;
}