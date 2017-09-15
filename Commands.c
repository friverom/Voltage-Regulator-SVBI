/*
 * File:   Commands.c
 * Author: Federico
 *
 * Created on August 10, 2017, 4:25 PM
 */


#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Regulator.h"
#include "PID_Controller.h"
#include <time.h>
#include "Filters.h"
#include "ior5e.h"
#include "DS1307.h"
#include "LCD2S.h"
//#include "Regulator.h"

void reset_var(char *);
void set_list(char *, char *);
void get_list(char *);
void cal_analog(char *, char *);
void time_set(char *);
void date_set(char *);
void lcd_display(char *, char *);

char *buffer;
extern union Digital_Inputs Inputs;
extern char rx_pointer;

void read_command(char *rx_buff, char *tx_buff) {

    char * ptr[3];
    char * sep_tok = " ";
    int i=0;
    
    buffer=tx_buff;
    ptr[i] = strtok(rx_buff, sep_tok);
    
    while (ptr[i] != NULL) {
      //  printf("%s\n", ptr[i]);
        ptr[++i] = strtok(NULL, sep_tok);
    }
    
    if(strcmp(ptr[0],"reset")==0){
        reset_var(ptr[1]);
    } else if(strcmp(ptr[0],"set")==0){
        set_list(ptr[1], ptr[2]);
        
    } else if(strcmp(ptr[0],"get")==0){
        get_list(ptr[1]);
    
    } 
    else if(strcmp(ptr[0],"cal")==0){
        cal_analog(ptr[1],ptr[2]);
    }
    else if(strcmp(ptr[0],"lcd")==0){
        lcd_display(ptr[1],ptr[2]);
    }
    else {
        sprintf(buffer,"Comando invalido\n");
        rx_pointer=0;
        TXIE=1;
    }
}

void lcd_display(char *var, char*data){
    
    int val;
        
    if(strcmp(var,"contrast")==0){
        val=atoi(data);
        LCD_SetContrast((char)val);
    }
    else if(strcmp(var,"backlight")==0){
        val=atoi(data);
        LCD_SetBackLight_Brightness((char)val);
    }
}
void reset_var(char *var){

    if(strcmp(var,"max")==0){
        set_max(Get_Voltage_Vin());
    }else if (strcmp(var,"min")==0){
        set_min(Get_Voltage_Vin());
    }else{
        sprintf(buffer,"Error\n");
        TXIE=1;
    }
}
void cal_analog(char *var, char *data){
    
    double val;
    
    if(strcmp(var,"v1_z")==0){
        val=atof(data);
        set_VL1_zero(val);
        Save_Variables();
    }
    else if(strcmp(var,"v2_z")==0){
        val=atof(data);
        set_VL2_zero(val);
        Save_Variables();
    }
    else if(strcmp(var,"v3_z")==0){
        val=atof(data);
        set_VL3_zero(val);
        Save_Variables();
    }
    else if(strcmp(var,"v1_s")==0){
        val=atof(data);
        set_VL1_span(val);
        Save_Variables();
    }
    else if(strcmp(var,"v2_s")==0){
        val=atof(data);
        set_VL2_span(val);
        Save_Variables();
    }
    else if(strcmp(var,"v3_s")==0){
        val=atof(data);
        set_VL3_span(val);
        Save_Variables();
    }
    else if(strcmp(var,"vin_s")==0){
        val=atof(data);
        set_Vin_span(val);
        Save_Variables();
    }
    else if(strcmp(var,"vin_z")==0){
        val=atof(data);
        set_Vin_zero(val);
        Save_Variables();
    }
    else{
        sprintf(buffer,"Parametro errado");
        TXIE=1;
    }
}
void set_list(char *var, char *data){
    
    double val;
    
    if(strcmp(var,"kp")==0){
        val=atof(data);
        set_PID_Kp((float)val);
        Save_Variables();
    }
    else if(strcmp(var,"ti")==0){
        val=atof(data);
        set_PID_Ki(val);
        Save_Variables();
    }
    else if(strcmp(var,"td")==0){
        val=atof(data);
        set_PID_Kd(val);
        Save_Variables();
    }
    else if(strcmp(var,"sample_time")==0){
        val=atof(data);
        set_PID_T(val);
        Save_Variables();
    }
    else if(strcmp(var,"setpoint")==0){
        val=atof(data);
        set_PID_setpoint(val);
        Save_Variables();
    }
    else if(strcmp(var,"deadband")==0){
        val=atof(data);
        set_Deadband(val);
        Save_Variables();
    }
    else if(strcmp(var,"rc")==0){
        val=atof(data);
        Set_LP_RC_constant(val);
        Save_Variables();
    }
    else if(strcmp(var,"max")==0){
        val=atof(data);
        set_max_VL(val);
        Save_Variables();
    }
    else if(strcmp(var,"min")==0){
        val=atof(data);
        set_min_VL(val);
        Save_Variables();
    }
    else if(strcmp(var,"time")==0){
        time_set(data);
    }
    else if(strcmp(var,"date")==0){
        date_set(data);
    }
    
    else{
        sprintf(buffer,"Parametro errado");
        TXIE=1;
    }
    
}

void get_list(char *var){

    time_t systime;
    struct tm *t;
    
    if(strcmp(var,"kp")==0){
        sprintf(buffer,"%.1f\n",get_PID_Kp());
        TXIE=1;
    } 
    else if(strcmp(var,"ti")==0){
        sprintf(buffer,"%.1f\n",get_PID_Ti());
        TXIE=1;
    }
    else if(strcmp(var,"td")==0){
        sprintf(buffer,"%.1f\n",get_PID_Td());
        TXIE=1;
    }
    else if(strcmp(var,"sample_t")==0){
        sprintf(buffer,"%.1f\n",get_PID_T());
        TXIE=1;
    }
    else if(strcmp(var,"setpoint")==0){
        sprintf(buffer,"%.1f\n",get_PID_setpoint());
        TXIE=1;
    }
    else if(strcmp(var,"deadband")==0){
        sprintf(buffer,"%.1f\n",get_PID_deadband());
        TXIE=1;
    }
    else if(strcmp(var,"rc")==0){
        sprintf(buffer,"%.2f\n",get_LP_RC_Constant());
        TXIE=1;
    }
    else if(strcmp(var,"max")==0){
        sprintf(buffer,"%.1f\n",get_max_VL());
        TXIE=1;
    }
    else if(strcmp(var,"min")==0){
        sprintf(buffer,"%.1f\n",get_min_VL());
        TXIE=1;
    }
    else if(strcmp(var,"modo")==0){
        if(Inputs.B0==1){
            sprintf(buffer,"manual\n");
            TXIE=1;
        }
        else{
            sprintf(buffer,"Auto\n");
            TXIE=1;
        }
    }
    else if(strcmp(var,"vc")==0){
        sprintf(buffer,"%.1f\n",Get_Control_Voltage());
        TXIE=1;
    }
    else if(strcmp(var,"vin")==0){
        sprintf(buffer,"%.1f\n",Get_Voltage_Vin());
        TXIE=1;
    }
    else if(strcmp(var,"date")==0){
        time(&systime);
        t=localtime(&systime);
        sprintf(buffer,"%d/%d/%d\n",t->tm_mday,t->tm_mon,t->tm_year+1900);
        TXIE=1;
    }
    else if(strcmp(var,"time")==0){
        time(&systime);
        t=localtime(&systime);
        sprintf(buffer,"%d:%d:%d\n",t->tm_hour,t->tm_min,t->tm_sec);
        TXIE=1;
    }
    else if(strcmp(var,"data")==0){
        if (Inputs.B0 == 1) {
            sprintf(buffer, "%.1f %.1f %.1f %.1f MANUAL\n", Get_Control_Voltage(), Get_Voltage_Vin(), get_PID_setpoint(), get_PID_output());
            TXIE = 1;
        }
        else {
            sprintf(buffer, "%.1f %.1f %.1f %.1f AUTO\n", Get_Control_Voltage(), Get_Voltage_Vin(), get_PID_setpoint(), get_PID_output());
            TXIE = 1;
        }
        
    }
    
    else if(strcmp(var,"pidout")==0){
        sprintf(buffer,"%.1f\n",get_PID_output());
        TXIE=1;
    }
    
    else {
        sprintf(buffer,"Error\n");
        TXIE=1;
    }

}
void date_set(char * dat){
    
    char * ptr[5];
    char * sep_tok = "/";
    int i=0;
    int data[4];
    struct Date_Data date;
    
     ptr[i] = strtok(dat, sep_tok);
    
    while (ptr[i] != NULL) {
        data[i]=atoi(ptr[i]);
     //   printf("%x\n", data[i]);
        ptr[++i] = strtok(NULL, sep_tok);
    }
    data[2]=data[2]-2000;
    for(i=0;i<4;i++){
        data[i]=intTObcd(data[i]);
    }
     date.date=data[0];
     date.month=data[1];
     date.year=data[2];
     date.day_w=data[3];
   //  printf("%x %x %x %x",date.date,date.month,date.year,date.day_w);
     
     set_date(&date);
}

void time_set(char *tim){
    char * ptr[3];
    char * sep_tok = ":";
    int i=0;
    int data[3];
    struct Time_Data hour;
    
    ptr[i] = strtok(tim, sep_tok);
    
    while (ptr[i] != NULL) {
        data[i]=intTObcd((int)atoi(ptr[i]));
    //    printf("%s\n", ptr[i]);
        ptr[++i] = strtok(NULL, sep_tok);
    }
    hour.hours=data[0];
    hour.minutes=data[1];
    hour.seconds=data[2];
    
  //  printf("%x %x %x",hour.hours,hour.minutes,hour.seconds);
    set_time(&hour);
        
}