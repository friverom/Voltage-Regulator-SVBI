/* Rutinas para manejo del reloj DS1307 */

#include <xc.h>
#include "i2c_FRM.h"
#include "DS1307.h"
#include <time.h>



/* Rutina para lectura del reloj en el DS1307 */

char get_time(char *Time_Data)
{
	ReadStr_I2C(MOD_ADDR,TIME_REG,Time_Data,3);
    return(0);
  		
}  	

char get_date(char *Date_Data){
    
    ReadStr_I2C(MOD_ADDR,DATE_REG,Date_Data,4);
    return(0);
}
  	
char set_time(char *Time_Data){
    
    WriteStr_I2C(MOD_ADDR,TIME_REG,Time_Data,3);
    return(0);
}

char set_date(char *Date_Data){
    
    WriteStr_I2C(MOD_ADDR,DATE_REG,Date_Data,4);
    return(0);
}

time_t time (time_t *t){
    
    struct tm DateTime;
    struct Time_Data time;
    struct Date_Data date;
    
    get_date(&date);
    get_time(&time);
    
    DateTime.tm_year=bcdTOint(date.year)+100;
    DateTime.tm_mon=bcdTOint(date.month)-1;
    DateTime.tm_mday=bcdTOint(date.date);
    DateTime.tm_wday=bcdTOint(date.day_w);
    DateTime.tm_hour=bcdTOint(time.hours);
    DateTime.tm_min=bcdTOint(time.minutes);
    DateTime.tm_sec=bcdTOint(time.seconds);
    
    *t=mktime(&DateTime);
    
    return(*t);
    
}

int bcdTOint(int bcdByte){
    
    return (((bcdByte & 0xF0) >> 4) * 10) + (bcdByte & 0x0F);
}

int intTObcd(int v){
int total = 0;
int resultbase = 1;
while(v > 0 ){
    total += resultbase * (v % 10);
    resultbase *= 16;
    v /= 10;
}

return total;
}
