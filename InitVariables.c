/*
 * File:   InitVariables.c
 * Author: Federico
 *
 * Created on August 8, 2017, 12:32 PM
 */


#include <xc.h>
#include "EEProm.h"
#include "Regulator.h"
#include "PID_Controller.h"
#include "Filters.h"

int Get_Variables(void) {
    
    union data_var data;
    
    rd_string_EE(0, Array_Size, &data);
    set_PID_Kp(data.kp);
    set_PID_Ki(data.ti);
    set_PID_Kd(data.td);
    set_PID_T(data.T_sample);
    set_PID_setpoint(data.set_point);
    set_Deadband(data.deadband);
    Set_LP_RC_constant(data.RC_Constant);
    set_max_VL(data.max_VL);
    set_min_VL(data.min_VL);
    set_VL1_zero(data.V1_zero);
    set_VL2_zero(data.V2_zero);
    set_VL3_zero(data.V3_zero);
    set_VL1_span(data.V1_span);
    set_VL2_span(data.V2_span);
    set_VL3_span(data.V3_span);
    set_Vin_zero(data.Vin_zero);
    set_Vin_span(data.Vin_span);
    
    return data.code;
}

void Save_Variables(void){
    
    union data_var data;
    
    data.code=CODE;
    data.kp=get_PID_Kp();
    data.ti=get_PID_Ti();
    data.td=get_PID_Td();
    data.T_sample=get_PID_T();
    data.set_point=get_PID_setpoint();
    data.deadband=get_PID_deadband();
    data.RC_Constant=get_LP_RC_Constant();
    data.max_VL=get_max_VL();
    data.min_VL=get_min_VL();
    data.V1_zero=get_VL1_zero();
    data.V2_zero=get_VL2_zero();
    data.V3_zero=get_VL3_zero();
    data.V1_span=get_VL1_span();
    data.V2_span=get_VL2_span();
    data.V3_span=get_VL3_span();
    data.Vin_zero=get_Vin_zero();
    data.Vin_span=get_Vin_span();
    wr_string_EE(0, Array_Size, &data);
}
