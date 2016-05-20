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

#define _XTAL_FREQ 8000000

void ADC_init(void) {
    ANSELbits.ANS4 = 1; //ch4 analogue
    
    ADCON0bits.ADFM = 1; //right justified
    ADCON0bits.VCFG = 0; //VDD = +VE
    ADCON1bits.ADCS = 0b101; //fosc/16//0b001; //Fosc/8
}

int ADC_read(int chan) {
    if (chan > 7) {
        return 0;
    }
    
    ADCON0bits.ADON = 0;
    ADCON0bits.CHS = chan; 
    ADCON0bits.ADON = 1;
    
    __delay_us(5); //Wait the acquisition time.
    
    ADCON0bits.GO = 1; //Start reading
    
    while (ADCON0bits.GO == 1) {}; //Wait until complete
    
    return (ADRESH<<8)+ADRESL; //8 bit registers, 10bit reading, combine them
}

int main()
{
    ADC_init();
    TRISC = 0xFF; //Reading channel 4. Its on TRISC so mark Whole port as input. Sledgehamme
    TRISCbits.TRISC5 = 0; //Mark RC5&4 as output for LED's
    TRISCbits.TRISC4 = 0;
    
    int adcResOne;
    int adcResTwo;

    while(1)
    {
        adcResOne = ADC_read(4); 

        if(adcResOne > 512)
            PORTCbits.RC5 = 1;      
        else
            PORTCbits.RC5 = 0;     
        
        adcResTwo = ADC_read(2);

        if(adcResTwo > 512)
            PORTCbits.RC4 = 1;      
        else
            PORTCbits.RC4 = 0;     
    }

    return 0;
}