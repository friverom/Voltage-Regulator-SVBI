
/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

float PID_Controller(float);
void reset_PID(void);
void set_PID_Kp(float);
void set_PID_Ki(float);
void set_PID_Kd(float);
void set_PID_T(float);
void set_PID_setpoint(float);
void set_Deadband(float);
float get_PID_Kp(void);
float get_PID_Ti(void);
float get_PID_Td(void);
float get_PID_T(void);
float get_PID_setpoint(void);
float get_PID_deadband(void);
float get_PID_output(void);