/*
 * File:   PassBandFilter.c
 * Author: Federico
 *
 * Created on August 3, 2017, 1:05 PM
 */


#include <xc.h>

static float k1=0; //Low pass RC constant
static float VL1_last;
static float VL2_last;
static float VL3_last;
static float Vin_last;

float get_LP_RC_Constant(void){
    return k1;
}
float LowPass_VL1(float val) {
       
    float Vout;
    
    Vout=k1*VL1_last+(1.0-k1)*val;
    VL1_last=Vout;
    return Vout;
}

float LowPass_VL2(float val) {
       
    float Vout;
    
    Vout=k1*VL2_last+(1.0-k1)*val;
    VL2_last=Vout;
    return Vout;
}

float LowPass_VL3(float val) {
       
    float Vout;
    
    Vout=k1*VL3_last+(1.0-k1)*val;
    VL3_last=Vout;
    return Vout;
}
float LowPass_Vin(float val) {
       
    float Vout;
    
    Vout=k1*Vin_last+(1.0-k1)*val;
    Vin_last=Vout;
    return Vout;
}

void Set_LP_RC_constant(float val){
    
    k1=val;
}

