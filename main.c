/*
 * File:   main.c
 * Author: Federico
 *
 * Created on August 1, 2017, 9:31 AM
 */


#include <xc.h>
#include "ConfigBits.h"
#include <stdio.h>
#include "ior5e.h"
#include "USART.h"
#include "i2c_FRM.h"
#include <time.h>
#include "LCD2S.h"
#include <string.h>
#include "Timers.h"
#include "Filters.h"
#include "Regulator.h"
#include "PID_Controller.h"
#include "Commands.h"

#define _XTAL_FREQ 10000000

#define CR 0x0d
#define ERROR 3
#define TX_RDY 2
#define RX_RDY 1
#define RX_IDLE 3
#define TX_IDLE 3

void check_V1_status( union data_check);
void check_V2_status( union data_check);
void check_V3_status( union data_check);
void Control_Loop(void);
void Manual_Control(void);
void Display_Data(void);
char check_start_flag(void);

char Int_Flag=0;
SplitInt value=61630;

union Data_out Outputs; //IOR5E Leds and relay outputs
union Digital_Inputs Inputs; //IOR5E Digital Inputs
char rx_buffer[20], tx_buffer[40];
char rx_flag,tx_flag;
char rx_pointer, tx_pointer;

char start_flag=0; //Bandera para retrasar control loop en arranque
char start_counter=0; //temporizador arranque
int start_value=0;

void main(void) {
   
    
    time_t sys_time;    //RTC time
    struct tm *t;   //RTC system time 
    int i,code;
    union data_check voltage_status; //Line voltages status variable
    float voltage, Control_Out;
 
    
    
    char *p = "      RD3 CA\nVR version 1.1 2017\n\n Iniciando Sistema\n";
    int p_len;
    char buffer[20];
    char BackLight=0;
    
    
       
    TRISA=0xff;
    TRISB=0xff;
    TRISC=0xff;
    
    //Inicializar USART 8 bits Async 9600 baudios
    OpenUSART(USART_TX_INT_OFF&USART_RX_INT_ON&USART_BRGH_HIGH&USART_CONT_RX&USART_EIGHT_BIT&USART_ASYNCH_MODE,64);
    rx_flag=0;
    rx_pointer=0;
    tx_flag=0;
    tx_pointer=0;
    for(i=0;i<10000;i++);
    
    //Inicializar BUS I2C como Master y 100KHz
    printf("Inicializando Bus I2C...");
    OpenI2C(MASTER,SLEW_OFF);
    printf("Listo.\n");
        
    //Leer fecha y hora del RTC
    time(&sys_time); //Leer fecha y hora del RTC
    printf("Inicializando fecha y hora del sistema.\n");
    
    //Inicializar IO de modulo IOR5E
    OpenIO5R();
    printf("Inicializando modulo IOR5...");
    printf("Listo.\n");
    OpenADC();
    printf("ADC sistema... Listo.\n");
    Outputs.Output=0;
    WriteIO5R(Outputs.Output); //Salidas en OFF
    
    set_max(0.0);
    set_min(200.0);
    
    time(&sys_time);
   // sys_time=1502549740;
    printf("%s\n",ctime(&sys_time));
        
    //Iniciar LCD Display
    LCD_Clear();
    p_len=strlen(p);
    LCD_WriteStr(p,p_len);
     for(i=0;i<500;i++){
        __delay_ms(10);
    }
    LCD_Clear();
    //Inicializar TIMER0 como base de tiempo
    value.tim_value=46004; //t=500 mseg
    Open_Timer0(ON&MODE_16_BIT&INT_CLK&INC_HL&PRE_ON&PRE_64 ,INT_EN,value.tim_value); 
    
    //Leer variables de EEPROM 
    code=Get_Variables(); //Si code=CODE, variables en EEPROM son validas
    //en caso contrario inicializar con valores Iniciales
    if (code != CODE) {
        set_PID_Kp(1.0);   //Contsante Proporcional del PID
        set_PID_Ki(0.0);    //Contsante de tiempo Integral del PID
        set_PID_Kd(0.0);    //Constante de tiempo derivativo de PID
        set_PID_T(0.5);     //Tiempo de muestreo
        set_PID_setpoint(118.0); //Setpoint de voltaje de linea
        set_Deadband(5.0);  //Banda muerta para error de control
        Set_LP_RC_constant(0.25);   //Constante RC filtro pasabajo
        set_max_VL(130.0);  //Maximo voltaje de salida para inhibir control
        set_min_VL(105.0);  //Minimo voltaje de salida para inhibir control
        set_VL1_zero(0.0);
        set_VL2_zero(0.0);
        set_VL3_zero(0.0);
        set_VL1_span(1.0);
        set_VL2_span(1.0);
        set_VL3_span(1.0);
        set_Vin_zero(0.0);
        set_Vin_span(1.0);
        Save_Variables();   //Salvar variables en EEPROM
    }
    //Calcular tiempo de estabilizacion de voltajes
    start_value = (int) (get_LP_RC_Constant()*4.0/0.5);
    
    //Habilitar Interrupts
    INTCONbits.PEIE=1;
    INTCONbits.GIE=1;
    
  
    while (1) {
        
        //Chequear si data en buffer de RX
        if(RCIE==0){
            if (rx_flag == RX_RDY) {
                rx_flag = RX_IDLE;
                rx_pointer = 0;
                while (TXIE == 1);
                read_command(rx_buffer, tx_buffer); //Procesar comando
                RCIE = 1;
            }
            else{
                rx_flag = RX_IDLE;
                rx_pointer = 0;
                RCIE = 1;
            }
            
        }
        //Corre rutina de principal si TIMER0
        if (Int_Flag == 1) {
            Int_Flag = 0;
           
            time(&sys_time);    //Actualizar Fecha y Hora del sistema
            
            //Leer voltajes de linea y de entrada
            voltage_status.data=Get_Voltage_Data();
            
            //Chequear si valores en rangos
            check_V1_status(voltage_status); //
            check_V2_status(voltage_status);
            check_V3_status(voltage_status);
            
            //Chequear si en modo AUTO, correr lazo de control
            Inputs.byte=Rd_Digital_Inputs();
            if(!Inputs.B0){
                
                if (check_start_flag()!=0) {
                    Control_Loop(); //Modo Auto activo
                    Outputs.led8 = 1;
                }
            } else {
                Manual_Control(); //Modo manual
                Outputs.led8=0;
            }
            // Chequear si reset de Max y Min
            if(Inputs.B1){
                set_min(Get_Voltage_Vin());
                set_max(Get_Voltage_Vin());
            }
            Display_Data(); //ACtualizar Display LCD
            Outputs.led7 = ~Outputs.led7;
            WriteIO5R(Outputs.Output); //Actualizar salidas
        }
    }

}

//Set Control Flag si timer de arranque es > RC*4/0.5
char check_start_flag(){

    if(start_flag==0){
        if(++start_counter>start_value+1)
            start_flag=1;
        else
            start_flag=0;
    }
    return start_flag;
}
//Reset valores de max y min de voltaje de entrada a media noche
void reset_max_min(void){
    
    time_t system_time;
    struct tm *t;
    int val;
    
    time(&system_time);
    t=localtime(&system_time);
    val=t->tm_hour+t->tm_min+t->tm_sec;
    
    if(val<5){
        
        set_max(Get_Voltage_Vin());
        set_min(Get_Voltage_Vin());
    }
}

//ACtualizar display LCD
void Display_Data(void){
    
    char buff_len,buffer[20];
    
    LCD_SetCursor_Position(1,1);
    buff_len=sprintf(buffer,"V_Out (LN1-LN2-LN3)");
    LCD_WriteStr(buffer,buff_len);
    
    LCD_SetCursor_Position(2,1);
    buff_len=sprintf(buffer,"%.1f",Get_Voltage_VL1());
    LCD_WriteStr(buffer,buff_len);
    
    LCD_SetCursor_Position(2,8);
    buff_len=sprintf(buffer,"%.1f",Get_Voltage_VL2());
    LCD_WriteStr(buffer,buff_len);
    
    LCD_SetCursor_Position(2,15);
    buff_len=sprintf(buffer,"%.1f",Get_Voltage_VL3());
    LCD_WriteStr(buffer,buff_len);
    
    LCD_SetCursor_Position(3,1);
    buff_len=sprintf(buffer,"V_In (Min Act Max)");
    LCD_WriteStr(buffer,buff_len);
    
    LCD_SetCursor_Position(4,1);
    buff_len=sprintf(buffer,"%.1f",get_min());
    LCD_WriteStr(buffer,buff_len);
    
    LCD_SetCursor_Position(4,8);
    buff_len=sprintf(buffer,"%.1f",Get_Voltage_Vin());
    LCD_WriteStr(buffer,buff_len);
    
    LCD_SetCursor_Position(4,15);
    buff_len=sprintf(buffer,"%.1f",get_max());
    LCD_WriteStr(buffer,buff_len);
}

//Control Manual. Deshabilitar salidas
void Manual_Control(void) {

    Outputs.led14 = 0;
    Outputs.led13 = 0;
    Outputs.rly1 = 0;
    Outputs.rly2 = 0;
    reset_PID();
    return;
}

//Control Automatico
void Control_Loop(void) {
    
    float voltage, Control_Out;
        
    voltage = Get_Control_Voltage();
    Control_Out = PID_Controller(voltage);

    if (voltage != 0.0) {
        if (Control_Out > get_PID_deadband()) {
            Outputs.led14 = 1;
            Outputs.led13 = 0;
            Outputs.rly1 = 1;
            Outputs.rly2 = 0;
        } else if (Control_Out<-get_PID_deadband()) {
            Outputs.led14 = 0;
            Outputs.led13 = 1;
            Outputs.rly1 = 0;
            Outputs.rly2 = 1;
        } else {
            Outputs.led14 = 0;
            Outputs.led13 = 0;
            Outputs.rly1 = 0;
            Outputs.rly2 = 0;
        }
    } else {
        Outputs.led14 = 0;
        Outputs.led13 = 0;
        Outputs.rly1 = 0;
        Outputs.rly2 = 0;
    }
    
}

void check_V1_status( union data_check status){
    
        
    if(status.VL1_Low==1){
                Outputs.led1=1;
                Outputs.led2=0;
                Outputs.led16=1;
                Outputs.rly3=1;
                
            }
            else if(status.VL1_High==1){
                Outputs.led1=0;
                Outputs.led2=1;
                Outputs.led16=1;
                Outputs.rly3=1;
               
            }
            else{
                Outputs.led1=0;
                Outputs.led2=0;
                Outputs.led16=0;
                Outputs.rly3=0;
                
            }
            
}

void check_V2_status( union data_check status){
    
        
    if(status.VL2_Low==1){
                Outputs.led3=1;
                Outputs.led4=0;
                Outputs.led16=1;
                Outputs.rly3=1;
            }
            else if(status.VL2_High==1){
                Outputs.led3=0;
                Outputs.led4=1;
                Outputs.led16=1;
                Outputs.rly3=1;
            }
            else{
                Outputs.led3=0;
                Outputs.led4=0;
                
            }
            
}

void check_V3_status( union data_check status){
    
       
        if(status.VL3_Low==1){
                Outputs.led5=1;
                Outputs.led6=0;
                Outputs.led16=1;
                Outputs.rly3=1;
            }
            else if(status.VL3_High==1){
                Outputs.led5=0;
                Outputs.led6=1;
                Outputs.led16=1;
                Outputs.rly3=1;
            }
            else{
                Outputs.led5=0;
                Outputs.led6=0;
                
            }
    
            
}

 void interrupt TimerInt(void) {
     
     char stat;
     
    if (TMR0IE && TMR0IF) {
        TMR0IF = 0;
        Int_Flag=1;
        TMR0H=value.HSB;
        TMR0L=value.LSB;
    }
    if(TXIE && TXIF){
        
        if(tx_buffer[tx_pointer]!=NULL){
            TXREG=tx_buffer[tx_pointer++];
            if(tx_pointer>40){
                tx_pointer=0;
                tx_flag=ERROR;
                TXIE=0;
            }
        } 
        else {
            TXREG==NULL;
            tx_pointer=0;
            tx_flag=TX_RDY;
            TXIE=0;
        }
    }
        
    if (RCIE && RCIF) {
        stat = RCSTA & 0x06;
        if (stat == 0) {
            rx_buffer[rx_pointer++] = RCREG;
            if (rx_pointer > 20) {
                rx_pointer = 0;
                rx_flag = RX_IDLE;
            }
            else if (rx_buffer[rx_pointer - 1] == CR) {
                rx_buffer[rx_pointer - 1] = NULL;
                rx_flag = RX_RDY;
                rx_pointer = 0;
                RCIE = 0;
            }

        } 
        else {
            RCSTAbits.CREN = 0;
            RCSTAbits.CREN = 1;
            rx_pointer = 0;
            rx_flag = RX_IDLE;
        }
    }
        
        
    }
    
  

