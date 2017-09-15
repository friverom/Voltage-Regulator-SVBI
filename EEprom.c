/* Rutinas para control escritura y lectura del EEPROM */

#include <xc.h>

char rd_byte_EE(char addr);
void rd_string_EE(char addr, char lenght, char *buffer);
void wr_byte_EE(char addr, char data);
void wr_string_EE(char addr, char lenght, char *buffer);


/* Escritura de un string al EEPROM */

void wr_string_EE(char addr, char lenght, char *buffer)
{
	char i;
	    
	for(i=0;i<lenght;i++)
		wr_byte_EE(addr++,*buffer++);
		
}
		
/* Lectura de un string del EEPROM */

void rd_string_EE(char addr, char lenght, char *buffer)
{
	char i;
	
	for(i=0;i<lenght;i++)
		*buffer++=rd_byte_EE(addr++);
		
}		
/* lectura de un byte del EEPROM */

char rd_byte_EE(char addr)
{
	EEADR = addr; /* Direccion a leer */
	EECON1bits.EEPGD = 0; /* Habilitar EEPROM */
    EECON1bits.CFGS=0;
	EECON1bits.RD = 1; /* Leer byte */
	return(EEDATA);
}	 

/* escribir un byte en EEPROM */

void wr_byte_EE(char addr, char data)
{
    EEADR = addr; /* address a escribir */
	EEDATA = data; /* data */
	EECON1bits.EEPGD = 0; /* Habilitar EEPROM */
    EECON1bits.CFGS=0;
	EECON1bits.WREN = 1; /* Habilitar escritura*/
	
	
	INTCONbits.GIE=0;
	    	
	EECON2 = 0x55; 
	EECON2 = 0xaa; 
	
	EECON1bits.WR = 1; /* escribir byte */
	INTCONbits.GIE=1;
	   	
	while (!PIR2bits.EEIF) /* esperar fin de escritura */
	;
	EECON1bits.WREN=0;
            
	PIR2bits.EEIF = 0; 
	
}