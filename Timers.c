/*
 * File:   Timers.c
 * Author: Federico
 *
 * Created on August 2, 2017, 5:12 PM
 */

#include "Timers.h"
#include <xc.h>

//Rutina para habilitar el Timer 0
void Open_Timer0(char config, char IE, int value) {
    
    SplitInt data;
    
    data.tim_value=value;
    T0CON=config;
    TMR0H=data.HSB;
    TMR0L=data.LSB;
    INTCONbits.TMR0IF=0;
    
    if(IE==1)
        INTCONbits.TMR0IE=1;
    
    else
        INTCONbits.TMR0IE=0;
    
    }
