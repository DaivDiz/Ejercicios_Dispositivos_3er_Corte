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

/* Configure the data bus and Control bus as per the hardware connection
   Dtatus bus is connected to PB0:PB7 and control bus PD0:PD4*/
#include "glcd.h"   //User defined LCD library which contains the lcd routines
#include "delay.h"

/* start the main program */
//#include "glcd.h"   

//////////////VARIABLES GLOBALES/////////////////////////////
long valPot;

//////////////VARIABLES CODIGO 1/////////////////////////////
int setpoint;
int min=0,max=1023;
int secc=8;
int val;
//////////////VARIABLES CODIGO 3/////////////////////////////
long temp;


int opc=4;
long map();
void tapas();

void main() 
{
    switch(opc){
        ///////////////////////GRAFICA////////////////////////////////////
        case 1:
            
            TRISE=0x03;
            TRISC=0x07;
            
            int xStart=15;
            int x=xStart;
                
            for(int i=1;i<128;i++){
                GLCD_SetCursor(7,i);
                if(i%10==0){
                    glcd_DataWrite(0xC0);
                }else{
                    glcd_DataWrite(0x80);      
                }
            }
            
            int P,PH,PL,den,maxF;
            while(1){
                
                for(int j=7;j>-1;j--){
                    GLCD_SetCursor(j,0);
                    if(j%2==0){
                        glcd_DataWrite(0xFF);   
                        GLCD_SetCursor(j,1);
                        glcd_DataWrite(0x01);
                    } else {
                        glcd_DataWrite(0xFF);
                    }
                }
                
                
                
                ADCON1=0x0E; 
                ADCON0=0x01;
                ADCON2=0xB1;

                ADCON0=0x03;


                if(ADCON0bits.GODONE!=0){
                    valPot=(ADRESH*255)+(ADRESL);
                    if(valPot>max){
                        valPot=max;
                    }
//                    valPot=map(valPot,0,1023,min,max);
        //            GLCD_Printf("ADRESH: %d\nADRESL: %d",ADRESH,ADRESL);

        //            GLCD_Printf("\n\nADRESL: %d",ADRESL);
                }
                
                setpoint=(PORTA>>1)+(PORTE<<5)+(PORTC<<7);
                
                for(int i=0;i<128;i++){
                    int F=setpoint/160;
                    GLCD_SetCursor(F,i);
                    glcd_DataWrite(0x80);
                }
//                int spAxis = map(setpoint,0,)
                
                val=(max/secc);
                
                x++;
                
                if(x>128){
                    x=xStart;
                }
                
                den=(max/64);
                
                PL=(valPot/den);
                
                PH=((PL*secc)/63);
                
                maxF=6;
                
                if(PH>maxF){
                    PH=maxF;
                }
                
                P=(valPot%secc);
                
                uint8_t PointPos=pow(2,P);
                
//                GLCD_SetCursor(0,0);
//                GLCD_Printf("Val:%4d",valPot);
//                GLCD_Printf("L:%1d",L);
//                GLCD_Printf("PL:%4d",P);
//                GLCD_Printf("PH:%4d",PH);
                
                GLCD_SetCursor(PH,x);
                glcd_DataWrite(PointPos);
//                __delay_ms(25);                    //delay de 50ms
            }
        break;
        
        ///////////////////////TAPAS////////////////////////////////////
        
        case 2:
            
            TRISE=0x03;
            TRISC=0x07;
            while(1){
                ADCON1=0x0E; 
                ADCON0=0x01;
                ADCON2=0xB1;

                ADCON0=0x03;


                if(ADCON0bits.GODONE!=0){
                    valPot=(ADRESH*255)+(ADRESL);
                }
                
                GLCD_SetCursor(0,0);
                
                if(valPot!=0){
                    GLCD_Printf("\nLuz: %d\n",valPot);  
                } else {
                    GLCD_Printf("\nLuz: %1d\n",valPot);  
                }
                
                for(int n=0; n<45;n++){
                    GLCD_Printf(" ");
                }
                GLCD_SetCursor(2,0);
                                
                if(valPot>835){
                    GLCD_Printf("\n-> No Hay Tapa!");  
                } else {
                    if(valPot>810&&valPot<=835){
                        GLCD_Printf("\nColor: Blanco");
                    } else {
                        if(valPot>790 && valPot<=810){
                            GLCD_Printf("\nColor: Amarillo");
                        } else {
                            if(valPot>650 && valPot<=680){
                                GLCD_Printf("\nColor: Azul Claro");
                            } else {
                                if(valPot>330 && valPot<=370){
                                    GLCD_Printf("\nColor: Verde Oscuro");
                                } else {
                                    if(valPot>100 && valPot<=165){
                                        GLCD_Printf("\nColor: Azul Oscuro");
                                    } else {
                                        GLCD_Printf("\n-> No Hay Protector!");
                                    }
                                }
                            }
                        }
                    }
                }
//                GLCD_Clear();
                    
                
                     //~ -> °

        //        GLCD_Printf("\nPortA: %d",(PORTA>>1));
        //        GLCD_Printf("\n\nPortE: %d",(PORTE<<5));        
        //        GLCD_Printf("\n\nPortE: %d",(PORTC<<7));
        //        GLCD_Printf("\n\nTotal: %d",setpoint);
            }
        break;
        
        ///////////////////////TEMPERATURA////////////////////////////////////
        
        case 3:
            TRISE=0x03;
            TRISC=0x07;
            while(1){
                ADCON1=0x0E; 
                ADCON0=0x01;
                ADCON2=0xB1;

                ADCON0=0x03;
                
                if(ADCON0bits.GODONE!=0){
                    valPot=(ADRESH*255)+(ADRESL);
                }
                
                temp=map(valPot,0,1023,300,1023);
                GLCD_SetCursor(0,0);
                GLCD_Printf("Temp: %d",temp);
                
                setpoint=(PORTA>>1)+(PORTE<<5)+(PORTC<<7);
                
                GLCD_SetCursor(1,0);
                if(setpoint<300){
                    GLCD_Printf("Error de SetPoint");
                } else {
                    GLCD_Printf("SetPoint: %d",setpoint);
                    GLCD_SetCursor(2,0);
                    if(temp>setpoint){
                        GLCD_Printf("Temperatura Elevada");                    }
                }
        break;
        }
            
        case 4:
            //Filas A1-A4   Outputs
            TRISA=0x21;

            //Columnas A5-E2    Inputs
            TRISE=0x07;
            
//            Switch On/Off
            TRISC=0xFF;
            
            ADCON1=0x0E;
            ADCON0=0x01;
            ADCON2=0xB1;
            
            
            
            GLCD_Init();
            
            int index=0;
            uint8_t pA=0x02;
            uint8_t pB;
            uint8_t v1;
            
            pA=0x02;
            int xM,yM,ayM;
            int xSC=0,ySC=3;
            char valM;
            
            long saldo;
            while(1){
                if(PORTCbits.RC0==0)    //Dejar PORTCbits.RCO==1
                {
                    while(1){
                        char codTec[4];
                        //Sensores
                        long LDR;
                        long LM35;
//              ********* Bloque de Sensor LDR **********
//                      ADCON1=0x0E;
                        ADCON0=0x01;
                        ADCON2=0xB1;
                        ADCON0bits.GODONE=1;

                        if(ADCON0bits.GODONE==1){
                            LDR=(ADRESH*255)+(ADRESL);
                        }
                        
                        GLCD_SetCursor(0,0);
                        GLCD_Printf("LDR: %d",LDR);
                        
//                        ADCON0=0x05;
//                        ADCON2=0xB1;
//                        ADCON0bits.GODONE=1;
//                        if(ADCON0bits.GODONE==1){
//                            LM35=(ADRESH*255)+(ADRESL);
//                        }
//                        
//                        GLCD_Printf("\nLM35: %d",LM35);
//                        
//                        __delay_ms(500);
                        
                        
                        //Blanco -> 10000
                        //Amarillo -> 5000
                        //Azul Claro -> 2000
                        //Verde Oscuro -> 1000
                        //Azul Oscuro -> 500

                        //Colores Tapas
                        //1. Blanco
                        //2. Amarillo
                        //3. Azul Claro
                        //4. Verde Oscuro
                        //5. Azul Oscuro

                        int codColor=0;

                        GLCD_SetCursor(5,0);
                        if(LDR>748){
                            GLCD_Printf("\n-> No Hay Tapa!");  
                            codColor=0;
                        } else {
                            if(LDR>715&&LDR<=730){
                                GLCD_Printf("\nColor: Blanco");
                                codColor=1;
                                saldo+=10000;
                            } else {
                                if(LDR>680 && LDR<=710){
                                    GLCD_Printf("\nColor: Amarillo");
                                    codColor=2;
                                    saldo+=5000;
                                } else {
                                    if(LDR>650 && LDR<=680){
                                        GLCD_Printf("\nColor: Azul Claro");
                                        codColor=3;
                                        saldo+=2000;
                                    } else {
                                        if(LDR>330 && LDR<=370){
                                            GLCD_Printf("\nColor: Verde Oscuro");
                                            codColor=4;
                                            saldo+=1000;
                                        } else {
                                            if(LDR>100 && LDR<=165){
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

                        ADCON0bits.ADON=0;
                        ADCON1=0x0F;
                        //Filas A1-A4   Outputs
                        TRISA=0x21;

                        //Columnas A5-E2    Inputs
                        TRISE=0x07;
//              *****************************************
                  
                        
//              ********* Bloque de Teclado Matricial **********
                        pB=~pA;

                        PORTA=pB;

                        switch(pA){
                            case 2:
                                xM=0;
                                break;
                            case 4:
                                xM=1;
                                break;
                            case 8:
                                xM=2;
                                break;
                            case 16:
                                xM=3;
                                break;
                        }

                        if(PORTAbits.RA5==0){
                            ayM=1;
                        } else {
                            if(PORTEbits.RE0==0){
                                ayM=2;
                            } else {
                                if(PORTEbits.RE1==0){
                                    ayM=3;
                                } else {
                                    if(PORTEbits.RE2==0){
                                        ayM=4;
                                    } else {
                                        ayM=0;
                                    }
                                }
                            }
                        }

                        yM=(ayM-1);

                        char comb[4][4]={
                            '1','2','3','A',
                            '4','5','6','B',
                            '7','8','9','C',
                            '*','0','#','D'
                        };

                        int cont=1;
                        int cond=ayM;
                        
                        for(int i=0;i<4;i++){
                            GLCD_SetCursor(7,(i*6));
                            GLCD_Printf(codTec[i]);
                        }
                        
                        if(ayM>0){
                            ySC=1;
                            GLCD_SetCursor(ySC,xSC);
                            valM=comb[xM][yM];
                            GLCD_Printf("%c",valM);
                            cont++;
                            ayM=1;
                            xSC+=6; //Dejarlo en 6 cuando se termine
                            
                            if(cont<5){
                                codTec[cont--]=valM;
                                GLCD_Printf("\nCont: %d",cont);
                            } else {
                                cont=1;
                            }
                            
                            if(xSC>18){
                                ySC++;
                                xSC=0;
                                if(ySC>7){
                                    ySC=3;
                                }
                            }
                            while(1){
                                if(PORTAbits.RA5==0){
                                    ayM=1;
                                } else {
                                    if(PORTEbits.RE0==0){
                                        ayM=2;
                                    } else {
                                        if(PORTEbits.RE1==0){
                                            ayM=3;
                                        } else {
                                            if(PORTEbits.RE2==0){
                                                ayM=4;
                                            } else {
                                                ayM=0;
                                            }
                                        }
                                    }
                                }

                                if(cond!=ayM){
                                    break;
                                }
                            }
                        }
        //              *******************************************

                                pA=(pA<<1);

                                index++;
                                if(index>3){
                                    index=0;
                                    pA=0x02;
                                }

        //              ********* Bloque de Switch On/Off *********
                                if(PORTCbits.RC0==1){
                                    GLCD_Clear();
                                    GLCD_SetCursor(0,0);
                                    GLCD_Printf("OFF");
                                    break;
                                }
        //              *******************************************             

        //              ********* Bloque de Switch Clear **********
                                if(PORTCbits.RC1==1){
                                    GLCD_Clear();
                                    __delay_ms(500);
                                }
        //              *******************************************    
                    }
                } 
            }
        break;
    }
}

long map(long x, long in_m, long in_M, long out_m, long out_M){
    return (x-in_m)*(out_M-out_m)/(in_M-in_m)+out_m;
}
 


/*
void ADC_Init()
{
    ADCON0 = 0x01;
    ADCON1 = 0x0E;
    ADCON2 = 0XB1;
}

unsigned int ADC_Read(unsigned char channel)
{
    if(channel > 7)
    {
        return 0;
    }
    
    DELAY_ms(2);
    ADCON0bits.GODONE = 1;
    while(ADCON0bits.GODONE);
    return ((ADRESH<<8)+ADRESL);
}

void main()
{
    unsigned int a; //variable donde se guardará la medición
    TRISA = 0xFF;   //Todos entradas en PORTA
 
    ADC_Init();     //llamamos la cofiguración ADC
    GLCD_Init();
    GLCD_Clear();
 
    while(1){
        a = ADC_Read(0); //Guarda en 'a' el resultado de la lectura
        GLCD_Printf("PortA: %d",a);        
        GLCD_SetCursor(0,0);
 }
}
*/
