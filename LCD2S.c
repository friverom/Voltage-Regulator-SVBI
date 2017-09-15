/*
 * File:   LCD2S.c
 * Author: Federico
 *
 * Created on August 1, 2017, 2:59 PM
 */


#include <xc.h>
#include "i2c_FRM.h"

void I2C_Send(char command){
    OpenI2C(MASTER, SLEW_OFF);
    IdleI2C();
    StartI2C();
    WriteI2C(0x50);
    Wait_AckI2C();
    WriteI2C(command);
    Wait_AckI2C();
    StopI2C();
    CloseI2C();
}
void LCD_WriteStr(char *Str, char Str_Size){
    
     char stat=0,i=0;
     
    OpenI2C(MASTER, SLEW_OFF);
    SSPADD=0x18;
    IdleI2C();

// Send Module Address    
    StartI2C();
    stat=WriteI2C(0x50);
    stat+=Wait_AckI2C();
    
// Point to Register Address to write    
    stat+=WriteI2C(0x80);
    stat+=Wait_AckI2C();
  
//Send string to I2C Bus    
    while(Str_Size--){
        stat+=WriteI2C(*Str++);
        stat+=Wait_AckI2C();
    }        
    StopI2C();
    CloseI2C();

}

void LCD_Clear(void) {
    
    I2C_Send(0x8c);
    
   }

void LCD_Shift_Right(void){
    
    I2C_Send(0x85);
    }

void LCD_SetBackLight_On(void){
    
    I2C_Send(0x28);
    }

void LCD_SetBackLight_Off(void){
    
    I2C_Send(0x20);
    }

void LCD_SetBackLight_Brightness(char brightness){
    
    OpenI2C(MASTER, SLEW_OFF);
    IdleI2C();
    StartI2C();
    WriteI2C(0x50);
    Wait_AckI2C();
    WriteI2C(0x81);
    Wait_AckI2C();
    WriteI2C(brightness);
    Wait_AckI2C();
    StopI2C();
    CloseI2C();
}

void LCD_MoveCursor_Forward(void){
    
    I2C_Send(0x09);
    }

void LCD_MoveCursor_Backward(void){
    
    I2C_Send(0x01);
    }

void LCD_BlinkBlockCursor_On(){
    
    I2C_Send(0x18);
    
}

void LCD_BlinkBlockCursor_Off(){
    
    I2C_Send(0x10);
    
}

void LCD_UnderLineCursor_On(){
    
    I2C_Send(0x19);
    
}

void LCD_UnderLineCursor_Off(){
    
    I2C_Send(0x11);
   
}

void LCD_Display_On(){
    
    I2C_Send(0x1a);
    
}

void LCD_Display_Off(){
    
    I2C_Send(0x12);
    
}

void LCD_SetContrast(char val){
    
    OpenI2C(MASTER, SLEW_OFF);
    IdleI2C();
    StartI2C();
    WriteI2C(0x50);
    Wait_AckI2C();
    WriteI2C(0x82);
    Wait_AckI2C();
    WriteI2C(val);
    Wait_AckI2C();
    StopI2C();
    CloseI2C();
}

void LCD_MoveCursor_Right(){
    
    I2C_Send(0x83);
    
}

void LCD_MoveCursor_Left(){
    
    I2C_Send(0x84);
    
}

void LCD_Shift_Left(){
    
    I2C_Send(0x86);
    
}

void LCD_Shift_Up(){
    
    I2C_Send(0x87);
    
}

void LCD_Shift_Down(){
    
    I2C_Send(0x88);
    
   }

void LCD_SetCursor_Position(char row, char column){
    
    OpenI2C(MASTER, SLEW_OFF);
    IdleI2C();
    StartI2C();
    WriteI2C(0x50);
    Wait_AckI2C();
    WriteI2C(0x8a);
    Wait_AckI2C();
    WriteI2C(row);
    Wait_AckI2C();
    WriteI2C(column);
    Wait_AckI2C();
    StopI2C();
    CloseI2C();
}