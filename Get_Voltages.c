/*
 * File:   Get_Voltages.c
 * Author: Federico
 *
 * Created on August 7, 2017, 12:08 PM
 */


#include <xc.h>

#include "Regulator.h"
#include "ior5e.h"
#include "Filters.h"

#define VL_N_MAX 130.0
#define VL_N_MIN 105.0
#define VL_L_MAX 250.0
#define VL_L_MIN 180.0
#define mask_VL1 0b00000011
#define mask_VL2 0b00001100
#define mask_VL3 0b00110000

float VL1, VL2, VL3, Vin;
static float VL1_zero,VL2_zero,VL3_zero,Vin_zero;
static float VL1_span,VL2_span,VL3_span,Vin_Span;
float VL_max, VL_min;
static float min;
static float max;

union data_check status;

float get_VL1_zero(void){
    return VL1_zero;
}

float get_VL2_zero(void){
    return VL2_zero;
}

float get_VL3_zero(void){
    return VL3_zero;
}

float get_VL1_span(){
    return VL1_span;
}

float get_VL2_span(void){
    return VL2_span;
}

float get_VL3_span(void){
    return VL3_span;
}
float get_Vin_span(void){
    return Vin_Span;
}
float get_Vin_zero(void){
    return Vin_zero;
}
void set_VL1_zero(float val){
    VL1_zero=val;
}

void set_VL2_zero(float val){
    VL2_zero=val;
}

void set_VL3_zero(float val){
    VL3_zero=val;
}
void set_Vin_zero(float val){
    Vin_zero=val;
}
void set_VL1_span(float val){
    VL1_span=val;
}

void set_VL2_span(float val){
    VL2_span=val;
}

void set_VL3_span(float val){
    VL3_span=val;
}
void set_Vin_span(float val){
    Vin_Span=val;
}


float get_max(void){
    return max;
}

float get_min(void){
    return min;
}
void set_max(float val){
    max=val;
}

void set_min(float val){
    min=val;
}
void set_max_VL(float val){
    VL_max=val;
}

void set_min_VL(float val){
    VL_min=val;
}

float get_max_VL(void){
    return VL_max;
}

float get_min_VL(void){
    return VL_min;
}
// Get Voltage L1-N
float Get_Voltage_VL1(void){
    
    return VL1;
}

//Get voltage L2-N
float Get_Voltage_VL2(void){
    
    return VL2;
}

//Get Voltage L3-N
float Get_Voltage_VL3(void){
    
    return VL3;
}

//Get Input voltage to transformer
float Get_Voltage_Vin(void){
    
    return Vin;
}

//Compute Control Voltage to PID module
float Get_Control_Voltage(void){

    float volt=0.0;
    char count=3;
    
    if(status.data&mask_VL1){
        count--;
    }
    else{
        volt=volt+Get_Voltage_VL1();
    }
    
    if(status.data&mask_VL2){
        count--;
    }
    else{
        volt=volt+Get_Voltage_VL2();
    }
    
    if(status.data&mask_VL3){
        count--;
    }
    else{
        volt=volt+Get_Voltage_VL3();
    }
    volt=volt/(float)count;
    return volt;
}

char Check_VL_N(float val){

    if(val>VL_max)
        return 1;
    else if(val<VL_min)
        return 2;
    else
        return 0;
        
}

char Get_Voltage_Data(void) {
    
    
    unsigned int v1;
    char check;
    

    status.data=0;
    v1 = Get_Analog_Channel(0);
    VL1 = (float) v1 / 1024.0 * 150.0;
    VL1=(VL1-VL1_zero)*VL1_span;
    VL1 = LowPass_VL1(VL1);
    check=Check_VL_N(VL1);
    
    switch(check){
        
        case 2:
            status.VL1_Low=1;
            status.VL1_High=0;
            break;
        
        case 1:
            status.VL1_High=1;
            status.VL1_Low=0;
            break;
            
        case 0:
            status.VL1_High=0;
            status.VL1_Low=0;
    }
    
    v1 = Get_Analog_Channel(1);
    VL2 = (float) v1 / 1024.0 * 150.0;
    VL2=(VL2-VL2_zero)*VL2_span;
    VL2 = LowPass_VL2(VL2);
    check=Check_VL_N(VL2);
    
    switch(check){
        
        case 2:
            status.VL2_Low=1;
            status.VL2_High=0;
            break;
        
        case 1:
            status.VL2_High=1;
            status.VL2_Low=0;
            break;
            
        case 0:
            status.VL2_High=0;
            status.VL2_Low=0;
    }
    
    v1 = Get_Analog_Channel(2);
    VL3 = (float) v1 / 1024.0 * 150.0;
    VL3=(VL3-VL3_zero)*VL3_span;
    VL3 = LowPass_VL3(VL3);
    check=Check_VL_N(VL3);
    
    switch(check){
        
        case 2:
            status.VL3_Low=1;
            status.VL3_High=0;
            break;
        
        case 1:
            status.VL3_High=1;
            status.VL3_Low=0;
            break;
            
        case 0:
            status.VL3_High=0;
            status.VL3_Low=0;
    }
    
    v1 = Get_Analog_Channel(3);
    Vin = (float) v1 / 1024.0 * 150.0;
    Vin=(Vin-Vin_zero)*Vin_Span;
    Vin = LowPass_Vin(Vin);
    
    if(Vin<min)
        min=Vin;
    
    if(Vin>max)
        max=Vin;
    
    return status.data;
}
