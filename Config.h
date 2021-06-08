//CONFIGURACIÓN DE BITS
//#pragma config PLLDIV = 1, CPUDIV = OSC1_PLL2, USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)
#pragma config FOSC = INTOSC_HS, FCMEN = OFF, IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
#pragma config PWRT = OFF, BOR = OFF, BORV = 0, VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)
#pragma config WDT = OFF, WDTPS = 32768, CCP2MX = ON, PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON, STVREN = OFF, LVP = OFF

#define _XTAL_FREQ 8000000