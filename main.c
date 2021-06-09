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
int column(),row(),products();
void tapas(),reset(),configPWM(),GLCD_ClearLine();


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
unsigned int saldo;
int stdSaldo;

//Sensores
long LDR;
int LM35=18;

//Productos
char prod[4][4]={   'A','D','C','B',
                    '5','9','7','C',
                    ' ',' ',' ','1',
                    ' ',' ',' ',' '};
int cost[4]={35,50,100,85};

void main() 
{
    xSC=sX;
    ySC=sY;
    //Filas A1-A4   Outputs
    TRISA=0x21;

    //Columnas A5-E2    Inputs
    TRISE=0x07;

    //Switch On/Off || Switch Reset ||  Led
    //RC0           || RC1          ||  RC2
    TRISC=0xFF;
    TRISCbits.RC6=1;
    TRISCbits.RC7=0;

    ADCON1=0x0E;
    ADCON0=0x01;
    ADCON2=0xB1;
    
    GLCD_Init();
    
    while(1){
        configPWM();
        PORTCbits.RC7=0;
//      ********* Bloque de Switch On/Off P1 *****
        if(PORTCbits.RC0==0)    //Dejar PORTCbits.RCO==1
        {          
            GLCD_DisplayLogo(LogoBitMap);
            __delay_ms(2500);
            GLCD_Clear();
            GLCD_SetCursor(0,0);
            GLCD_Printf("     Bienvenido al \n dispensador MLP3000");
            __delay_ms(3500);
            GLCD_Clear();
            PORTCbits.RC7=1;
            __delay_ms(500);
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
                tapas(LDR,6,0);         //tapas(long val,int y, int x)
                
                ADCON0bits.ADON=0;

//      ********* Bloque de Sensor LM35 ****************
                GLCD_SetCursor(7,103);
                GLCD_Printf("18~C");
                
//      ************************************************          
                
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
                
                char ped[4];
                
                if(ayM>0){
                    if(xSC<(sX+(nC*6)))
                    {
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
                    
                    if((contChar)>4){
                        int x=0;
//                        GLCD_SetCursor(4,0);
                        for(int i=0;i<4;i++){
                            for(int j=0;j<4;j++){
                                char product=prod[i][j];
                                char p=codTec[i];
                                if(p==product){
//                                    GLCD_SetCursor(4,0);
                                    ped[x]=p;
//                                    GLCD_Printf("%c",p);
                                    __delay_ms(1000);
                                    x++;
                                } 
                            }
                        }
                        int p=products(ped);
                        int c=cost[p-1];
                        reset(0);
                        GLCD_SetCursor(4,0);
                        GLCD_Printf("Precio: %d00",c);
                        contChar=1;
                        if(saldo>=c){
                            __delay_ms(200);
                            saldo-=c;
                            stdSaldo=1;
                        } else {
                            stdSaldo=0;
                        }
                    }
                }
                
                
                
                GLCD_SetCursor(3,0);
                if(stdSaldo){
                    if(saldo!=0){
                        GLCD_Printf("Saldo: $%d00",saldo);
                    } else{
                        GLCD_Printf("Saldo: $%1d",saldo);
                    }
                } else {
                    GLCD_Printf("Saldo Insuficiente");
                }
                
                GLCD_SetCursor(4,0);
                GLCD_Printf("Precio: ");
                
                if(saldo==0){
                    GLCD_SetCursor(5,0);
                    GLCD_Printf("Ingrese saldo.");
                } else {
                    GLCD_SetCursor(5,0);
                    GLCD_Printf("                                           ");
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
                if(PORTCbits.RC6==1){
                    reset(1);
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
void reset(int yn){
    if(yn==1){
        for(int i=0; i<4;i++){
            GLCD_SetCursor(sY,(sX+(i*6)));
            GLCD_Printf(" ");
            xSC=sX;
            contChar=1;
        }
    }
    
    if(yn==0){
        for(int i=0; i<4;i++){
            GLCD_SetCursor(sY,(sX+(i*6)));
            GLCD_Printf(" ");
            xSC=sX;
//            contChar=1;
        }
    }
    
//    __delay_ms(500);
}
//*********************************************
long map(long x, long in_m, long in_M, long out_m, long out_M){
    return (x-in_m)*(out_M-out_m)/(in_M-in_m)+out_m;
}

void tapas(long val, int y, int x){
    int ranges[10]={    335,345,        //Blanco
                        260,275,        //Amarillo
                        175,190,        //Azul Claro
                        45,55,          //Verde Oscuro
                        15,25};         //Azul Oscuro
    GLCD_SetCursor(y,x);
//    if(val>748){
//        GLCD_Printf("\n-> No Hay Tapa!");  
//        codColor=0;
//    } else {
        if(val>ranges[0]&&val<=ranges[1]){
            GLCD_Printf("\nColor: Blanco");
            codColor=1;
            saldo+=100;
        } else {
            if(val>ranges[2] && val<=ranges[3]){
                GLCD_Printf("\nColor: Amarillo");
                codColor=2;
                saldo+=50;
            } else {
                if(val>ranges[4] && val<=ranges[5]){
                    GLCD_Printf("\nColor: Azul Claro");
                    codColor=3;
                    saldo+=20;
                } else {
                    if(val>ranges[6] && val<=ranges[7]){
                        GLCD_Printf("\nColor: Verde Oscuro");
                        codColor=4;
                        saldo+=10;
                    } else {
                        if(val>ranges[8] && val<=ranges[9]){
                            GLCD_Printf("\nColor: Azul Oscuro");
                            codColor=5;
                            saldo+=5;
                        } 
//                        else {
//                            GLCD_Printf("\nSin Protector");
//                            codColor=0;
//                        }
//                    }
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

void configPWM(){
    PR2=0xFF;
    CCPR1L=0x02;
    CCP1CONbits.DC1B0=0;
    CCP1CONbits.DC1B1=0;
    
    TRISCbits.RC2=0;
    T2CONbits.T2CKPS=3;
    
    CCP1CONbits.CCP1M0=1;
    CCP1CONbits.CCP1M1=1;
    CCP1CONbits.CCP1M2=1;
    CCP1CONbits.CCP1M3=1;
    
    TMR2=0;
    
    T2CONbits.TMR2ON=1;
}
 
int products(char product[]){
    switch(product[0]){
        case 'A':
            if(product[1]=='5'){
                return 1;
            }
            break;
        case 'B':
            if(product[1]=='C'){
                if(product[2]=='1'){
                    return 4;
                }
            }
            break;
        case 'C':
            if(product[1]=='7'){
                return 3;
            }
            break;
        case 'D':
            if(product[1]=='9'){
                return 2;
            }
            break;
        default:
            return 0;
    }
}

void GLCD_ClearLine(int y){
    if(y<8 && y>-1){
        for(int i=0;i<128;i++){
            GLCD_SetCursor(y,i);
            glcd_DataWrite(0x00);
        }
    }
}