/*
 * File:   PID_Controller.c
 * Author: Federico Rivero
 *
 * Created on June 26, 2017, 3:06 PM
 */


#include <xc.h>
#include <stdlib.h>
#include "PID_Controller.h"

static float Kp;   //Proportional Gain
static float ti;   //Integral time constant  
static float td;   //Derivative time constant
static float T_sample; //PID running time Period.
float output;
float error;
static float integral;
static float last_output;
static float last_error;
static float last_integral;
static float setpoint;
static float deadband;

float get_PID_output(void){
    return last_output;
}

float get_PID_Kp(void){
    return Kp;
}

float get_PID_Ti(void){
    return ti;
}

float get_PID_Td(void){
    return td;
}

float get_PID_T(void){
    return T_sample;
}

float get_PID_setpoint(void){
    return setpoint;
}

float get_PID_deadband(void){
    return deadband;
}
/* Returns PID output base on the standard equation
 out=Kp*(error+ti*error_integral+td*error_derivative */
float PID_Controller(float val){
    float abs_error; //to calculate deadband limit

//PID evaluation
    error = setpoint - val;
    if(ti!=0){
        integral = (last_integral + error * T_sample*ti);
    }
    else{
        integral=0.0;
    }
    output = Kp * (error + integral + td * (error - last_error) / T_sample);
    last_output = output;
    last_error = error;
    last_integral = integral;
    return output;
}

//Reset PID Controller
void reset_PID(void){
    last_output=0.0;
    last_error=0.0;
    last_integral=0.0;
            
}

//Set deadband. val is in %
void set_Deadband(float val){
    deadband=val;
    
}

//Set proportional gain Kp
void set_PID_Kp(float val){
    Kp=val;
    last_integral=0.0;
    return;
}

//Set integral time constant
void set_PID_Ki(float val){
    ti=val;
    return;
}

//Set derivative time constant
void set_PID_Kd(float val){
    td=val;
    return;
}

//Set sampling time in segs.
void set_PID_T(float val){
    T_sample=val;
    return;
}

//Set setpoint
void set_PID_setpoint(float val){
    setpoint=val;
    return;
}