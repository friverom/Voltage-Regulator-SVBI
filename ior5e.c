#include <xc.h>
#include <pic18f258.h>
#include "ior5e.h"

#define _XTAL_FREQ 10000000

/* Rutina para inicializacion de las salidas del IOR5E */

void OpenIO5R(void)
{
		
	TRISCbits.TRISC0=0;
	TRISCbits.TRISC1=0;
	TRISCbits.TRISC2=0;
	TRISCbits.TRISC5=0;
    
    TRISBbits.TRISB0=1;
    TRISBbits.TRISB1=1;
    TRISBbits.TRISB4=1;
    TRISBbits.TRISB5=1;
	
	Shift_Reg_Clock=1;
	Reg_Clock=1;
	Out_Enable=1;
	
	WriteIO5R(0x00000000);
	Out_Enable=0;
}

	
/* Rutina para escritura en la salidas del IOR5E */

void WriteIO5R(unsigned long int data)
{
	char bits=0;
	unsigned long int temp;
	
	temp=data;
	
	for(bits=0;bits<24;bits++)
	{
		temp=data & 0x80000000;
		
		if(temp==0)
			Serial_Data=0;
		else
			Serial_Data=1;

// Generar pulso de clock para el shift register
		
		Shift_Reg_Clock=0;
		__delay_us(2);
		Shift_Reg_Clock=1;
			
		data=data<<1;
	}
// Generar pulso de clock para el registro de salida

	Reg_Clock=0;
	__delay_us(2);
	Reg_Clock=1;
}	

// Rutina para lectura de Puerto B

char Rd_Digital_Inputs(void)
{
    union Digital_Inputs Port_RD, Temp_RD;
    
    Temp_RD.byte=PORTB;
    
    Port_RD.B0=!Temp_RD.B0;
    Port_RD.B1=!Temp_RD.B1;
    Port_RD.B2=!Temp_RD.B4;
    Port_RD.B3=!Temp_RD.B5;
    
    return(Port_RD.byte);
}


// Rutina para inicializar conversor A/D

void OpenADC(void){
    
    ADCON0=0x81; //Clock Fosc/64 y habilitar ADC
    ADCON1=0xc2; // $ entradas analogicas
}


// Rutina para lectura valor analogico
unsigned int Get_Analog_Channel (char channel)
{
    union ADC {
        unsigned int Result;
        struct {
        unsigned char LSB;
        unsigned char MSB;
        };
    };
    
    union ADC ADC_data;
    char status;
   
    ADCON0bits.ADON=1;
    
    ADCON0bits.CHS=channel;
    __delay_us(15);
    ADCON0bits.GO=1;
      
    while(ADCON0bits.GO);
          
    ADC_data.LSB=ADRESL;
    ADC_data.MSB=ADRESH;
   
    return(ADC_data.Result);
   
}

