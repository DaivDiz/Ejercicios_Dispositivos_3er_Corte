#include <pic18f4550.h>
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//CONFIGURACIÓN DE BITS
//#pragma config PLLDIV = 1, CPUDIV = OSC1_PLL2, USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)
#pragma config FOSC = HS, FCMEN = OFF, IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)
#pragma config PWRT = OFF, BOR = OFF, BORV = 0, VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)
#pragma config WDT = OFF, WDTPS = 32768, CCP2MX = ON, PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON, STVREN = OFF, LVP = OFF

#define _XTAL_FREQ 16000000

#include "glcd.h" 
#include "delay.h"

//Funciones
long map();
void tapas(),reset();
int column(),row();

//Multiplexor
uint8_t pA=0x02;
uint8_t pB;
uint8_t v1;
int index=0;

//Teclado
int xM,yM,ayM,cond;
char comb[4][4]={   '1','2','3','A',
                    '4','5','6','B',
                    '7','8','9','C',
                    '*','0','#','D' };

//Visualización Dinamica (Cursor)
int sX=40,sY=1,nC=4,xSC,ySC,contChar=1;
char valM;

//Codigos
int codColor;
char codTec[4];

//Saldo
int saldo;

//Sensores
long LDR;

//Productos
char prod[4][3]={   'A','B','C','B',
                    '5','9','7','C',
                    '' ,'' , '','1' };

void main() 
{
    xSC=sX;
    ySC=sY;
    //Filas A1-A4   Outputs
    TRISA=0x21;

    //Columnas A5-E2    Inputs
    TRISE=0x07;

    //Switch On/Off
    TRISC=0xFF;

    ADCON1=0x0E;
    ADCON0=0x01;
    ADCON2=0xB1;

    GLCD_Init();
    while(1){
        
//      ********* Bloque de Switch On/Off P1 *****
        if(PORTCbits.RC0==0)    //Dejar PORTCbits.RCO==1
        {
//      ******************************************
            while(1){
//      ********* Bloque de Sensor LDR **********
                ADCON0=0x01;
                ADCON2=0xB1;
                ADCON0bits.GODONE=1;

                if(ADCON0bits.GODONE==1){
                    LDR=(ADRESH*255)+(ADRESL);
                }

                GLCD_SetCursor(0,0);
                GLCD_Printf("LDR: %d",LDR);
                tapas(LDR,5,0);         //tapas(long val,int y, int x)
                
                ADCON0bits.ADON=0;


//      ********* Bloque de Multiplexación P1***********
                pB=~pA;
                PORTA=pB;
//      ************************************************
                
//      ********* Bloque de Teclado Matricial **********
                
            //Control de Estados de teclado
                ayM=column();
                
            //Control de Teclas
                xM=row(pA);
                yM=(ayM-1);

                cond=ayM;
                
                GLCD_SetCursor(1,0);
                GLCD_Printf("Code: ");
                
                if(ayM>0 && xSC<(sX+(nC*6))){
                    GLCD_SetCursor(ySC,xSC);
                    valM=comb[xM][yM];
                    GLCD_Printf("%c",valM);
                    
                //Control de Cursor
                    ySC=1;
                    xSC+=6; //Dejarlo en 6 cuando se termine
                    
                //Control de Texto Ingresado
                    codTec[contChar-1]=valM;
                            
                //Contador de Caracteres
                    contChar++;
                    
                    while(1){
                        ayM=column();
                        if(cond!=ayM){
                            break;
                        }
                    }
                }
                GLCD_SetCursor(2,0);
                GLCD_Printf("Cont: %1d",contChar);
                if(contChar>4){
                    int c;
                    char v;
                    while(1){
                        v=codTec[c];
                        GLCD_SetCursor(3,0);
                        GLCD_Printf("%c",v);
                        c++;
                        if(c>3){
                            c=0;
                        }
                        __delay_ms(500);
                    }
                }
                
//      *********************************************

//      ********* Bloque de Switch On/Off P2*********
                if(PORTCbits.RC0==1){
                    GLCD_Clear();
                    GLCD_SetCursor(0,0);
                    GLCD_Printf("OFF");
                    break;
                }
//      *********************************************       
                if(PORTCbits.RC1==1){
                    reset();
                }
//      ********* Bloque de Multiplexación P2********
                pA=(pA<<1);

                index++;
                if(index>3){
                    index=0;
                    pA=0x02;
                }
//      *********************************************
            }
        } 
    }
}

//********* Bloque de Switch Clear ************
void reset(){
//        GLCD_Clear();
    for(int i=sX; i<(sX+(nC*6));i+=(nC*6)){
        GLCD_SetCursor(sY,i);
        GLCD_Printf(" ");
    }
    __delay_ms(500);
}
//*********************************************
long map(long x, long in_m, long in_M, long out_m, long out_M){
    return (x-in_m)*(out_M-out_m)/(in_M-in_m)+out_m;
}

void tapas(long val, int y, int x){
    GLCD_SetCursor(y,x);
    if(val>748){
        GLCD_Printf("\n-> No Hay Tapa!");  
        codColor=0;
    } else {
        if(val>715&&val<=730){
            GLCD_Printf("\nColor: Blanco");
            codColor=1;
            saldo+=10000;
        } else {
            if(val>680 && val<=710){
                GLCD_Printf("\nColor: Amarillo");
                codColor=2;
                saldo+=5000;
            } else {
                if(val>650 && val<=680){
                    GLCD_Printf("\nColor: Azul Claro");
                    codColor=3;
                    saldo+=2000;
                } else {
                    if(val>330 && val<=370){
                        GLCD_Printf("\nColor: Verde Oscuro");
                        codColor=4;
                        saldo+=1000;
                    } else {
                        if(val>100 && val<=165){
                            GLCD_Printf("\nColor: Azul Oscuro");
                            codColor=5;
                            saldo+=500;
                        } else {
                            GLCD_Printf("\nSin Protector");
                            codColor=0;
                        }
                    }
                }
            }
        }
    }
}

int row(int portA){
    switch(portA){
        case 2:
            return 0;
            break;
        case 4:
            return 1;
            break;
        case 8:
            return 2;
            break;
        case 16:
            return 3;
            break;
    }
}

int column(){
    if(PORTAbits.RA5==0){
        return 1;
    } else {
        if(PORTEbits.RE0==0){
            return 2;
        } else {
            if(PORTEbits.RE1==0){
                return 3;
            } else {
                if(PORTEbits.RE2==0){
                    return 4;
                } else {
                    return 0;
                }
            }
        }
    }
}
 
