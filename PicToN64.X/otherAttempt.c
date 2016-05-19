/*
 We don't talk about this one.
 
// PIC16F688 Configuration Bit Settings
#define _XTAL_FREQ 4000000
#include <pic16f688.h>
#include <xc.h>

// CONFIG
#pragma config FOSC = INTOSCIO // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Detect (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)

void ADC_Init();
int Read_Horz(void);
int Read_Vert (void);
int Read_ADC(int channel);

void ADC_Init()
{
    ADCON1 = 0b01010000;
    //ADCON0 = 0b0000001;
  //ADCON0 = 0x81;               //Turn ON ADC and Clock Selection
  //ADCON1 = 0x10;//0x00;               //All pins as Analog Input and setting Reference Voltages
}
int Read_Vert (void) {
    int horizAvg = 0;
    ADCON0 = 0b00010001; //setup on channel 4
    
    ADCON0 = ADCON0 | 0b00000010; // set go to 1 and make read
    
    while((ADCON0 >> 7) & 1); //wait untill godone is false
    
    return (ADRESH >> 1) - 127; //combine value cnter arund 0
ADCON0 = 0b00010001; //ch4
    ADCON0 = ADCON0 | 0b00000010;
    
    while((ADCON0 >> 7) & 1); //wait untill godone is false
   // __delay_ms(2);
    horizAvg = ((ADRESH<<8)+ADRESL); //center around zero
    
    return horizAvg;
            
    ADCON0 = 0b00010001; //ch2
    __delay_ms(2);
    ADCON0 = ADCON0 | 0b00000010;
    
    while((ADCON0 >> 7) & 1); //wait untill godone is false
    
    //return ((ADRESH<<8)+ADRESL) - 511; //center around zero
    return ADRESH - 127;    
}

int Read_Horz(void) {
    ADCON0 = 0b00001001; //ch2
    __delay_ms(2);
    ADCON0 = ADCON0 | 0b00000010;
    
    while((ADCON0 >> 7) & 1); //wait untill godone is false
    
    //return ((ADRESH<<8)+ADRESL) - 511; //center around zero
    //return ADRESH - 127;
    //return ((256*ADRESH) + ADRESL) - 511;
    //return (int)ADRESH + ((int)ADRESL * 256);
    return (((int)ADRESH<<8)+(int)ADRESL);
}

int Read_ADC(int channel) {
    if (channel > 7)
        return 0;
    
    ADCON0 &= 0xC3;              //Clearing channel selection bits
    ADCON0 |= channel<<2;        //Setting channel selection bits
    __delay_ms(2);               //Acquisition time to charge hold capacitor
    GO_nDONE = 1;                //Initializes A/D conversion
    while(GO_nDONE);             //Waiting for conversion to complete
    return ((ADRESH<<8)+ADRESL); //Return result
    
    ADCON0 = 0b00000001; //reset and enabled left justified, go off
    ADCON0 |= channel<<2; //set channel
    __delay_ms(2); //Delay acquasision time
    GO_nDONE = 1; //start conversion
    while(GO_nDONE); //wait till complete
    return ADRESH;
}

void main()
{               //Port C as Output
    ADC_Init();
    int vertCenter = 0;
    int vertCurrent = 0;
    int horzCenter = 0;
    int deadzoneUpper = 0;
    int deadzoneLower = 0;
    ANSEL = 0b00010100;
    TRISA = 0b111111;                 //Analog pins as Input
    TRISC = 0b000000;  
    //RC5 = 0;
    // RC4 = 0;
    //__delay_ms(2000);
        horzCenter = Read_Horz();
        //vertCenter = Read_Vert();
   do
    {
        RC5 = 1;
        if (Read_Horz() > 1000)
            RC5 = 0;
        
        __delay_ms(300);
       int val = Read_Horz();
       if (deadzoneUpper != val) {
           RC5 = 1;
           deadzoneUpper = val;
       }
      if (val > horzCenter + 50) {
          RC3 = 1;
          RC2 = 0;
      } else if (val < horzCenter - 50){
          RC3 = 0;
          RC2 = 1;
      } else {
          RC3 = 0;
          RC2 = 0;
      }
      
       if (Read_Vert() > vertCenter + 10) {
          RC5 = 1;
          RC4 = 0;
      } else if (Read_Vert() < vertCenter - 10){
          RC5 = 0;
          RC4 = 1;
      } else {
          RC5 = 0;
          RC4 = 0;
      }
        
        horzCurrent = Read_Horz();
      if (horzCurrent > vertDeadZone) {
          RC5 = 1;
          RC4 = 0;
      } else if(horzCurrent < vertDeadZone){
          RC5 = 0;
          RC4 = 1;
      } else {
          RC5 = 0;
          RC4 = 0;
      }
      //__delay_ms(100);            //Delay
    }while(1);                    //Infinite Loop
}*/