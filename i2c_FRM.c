#include <xc.h>
#include "i2c_FRM.h"

/********************************************************************
*   Function Name:  OpenI2C                                         *
*   Return Value:   void                                            *
*   Parameters:     SSP peripheral setup bytes                      *
*   Description:    This function sets up the SSP module on a       * 
*                   PIC18CXXX device for use with a Microchip I2C   *
*                   EEPROM device or I2C bus device.                *
********************************************************************/
void OpenI2C( unsigned char sync_mode, unsigned char slew )
{
  SSPSTAT &= 0x3F;                // power on state 
  SSPCON1 = 0x00;                 // power on state
  SSPCON2 = 0x00;                 // power on state
  SSPCON1 |= sync_mode;           // select serial mode 
  SSPSTAT |= slew;                // slew rate on/off 

  DDRCbits.RC3 = 1;               // Set SCL (PORTC,3) pin to input
  DDRCbits.RC4 = 1;               // Set SDA (PORTC,4) pin to input

  SSPCON1 |= SSPENB;              // enable synchronous serial port 
}

/********************************************************************
*     Function Name:    IdleI2C                                     *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      Test and wait until I2C module is idle.     *
********************************************************************/
// #undef IdleI2C
void IdleI2C( void )
{
  while ( ( SSPCON2 & 0x1F ) | ( SSPSTATbits.R_W ) )
     continue;
}

/********************************************************************
*     Function Name:    StartI2C                                    *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      Send I2C bus start condition.               *
********************************************************************/
//#undef StartI2C

void StartI2C(void) {
    char IE_stat;

    IE_stat = INTCON;
    INTCONbits.GIE = 0;
    SSPCON2bits.SEN = 1; // initiate bus start condition
    PIR1bits.SSPIF = 0;
    while (!PIR1bits.SSPIF);
    INTCON = IE_stat;
}
/********************************************************************
 *     Function Name:    WriteI2C                                    *
 *     Return Value:     Status byte for WCOL detection.             *
 *     Parameters:       Single data byte for I2C bus.               *
 *     Description:      This routine writes a single byte to the    * 
 *                       I2C bus.                                    *
 ********************************************************************/
unsigned char WriteI2C(unsigned char data_out) {
    char IE_stat;
    SSPBUF = data_out; // write single byte to SSPBUF
    if (SSPCON1bits.WCOL) // test if write collision occurred
        return ( -1); // if WCOL bit is set return negative #
    else {

        IE_stat = INTCON;
        INTCONbits.GIE = 0;
        PIR1bits.SSPIF = 0;
        while (!PIR1bits.SSPIF); // wait until write cycle is complete    
        INTCON = IE_stat;
        return ( 0); // if WCOL bit is not set return non-negative #
    }
}

/********************************************************************
*     Function Name:    Wait_AckI2C                                 *
*     Return Value:     -1 if Not Ack                               *
*     Parameters:       void                                        *
*     Description:      Initiate ACK bus condition.                 *
********************************************************************/
//#undef Wait_AckI2C
char Wait_AckI2C( void )
{
  if(SSPCON2bits.ACKSTAT)
	return(-1);
  else
  	return(0);
}

/********************************************************************
*     Function Name:    RestartI2C                                  *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      Send I2C bus restart condition.             *
********************************************************************/
//#undef RestartI2C

void RestartI2C(void) {
    char IE_stat;
    IE_stat = INTCON;
    INTCONbits.GIE = 0;
    SSPCON2bits.RSEN = 1; // initiate bus restart condition
    PIR1bits.SSPIF = 0;
    while (!PIR1bits.SSPIF);
    INTCON = IE_stat;
    ;
}
/********************************************************************
 *     Function Name:    ReadI2C                                     *
 *     Return Value:     contents of SSPBUF register                 *
 *     Parameters:       void                                        *
 *     Description:      Read single byte from I2C bus.              *
 ********************************************************************/
unsigned char ReadI2C(void) {
    char IE_stat;
    
    IE_stat = INTCON;
    INTCONbits.GIE = 0;
    SSPCON2bits.RCEN = 1; // enable master for 1 byte reception
    PIR1bits.SSPIF = 0;
    while (!PIR1bits.SSPIF); // wait until byte received
    INTCON = IE_stat;
    return ( SSPBUF); // return with read byte 
}

/********************************************************************
*     Function Name:    AckI2C1                                      *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      Initiate ACK bus condition.                 *
********************************************************************/
//#undef AckI2C

void AckI2C(void) {
    char IE_stat;
    
    IE_stat = INTCON;
    INTCONbits.GIE = 0;
    SSPCON2bits.ACKDT = 0; // set acknowledge bit state for ACK
    SSPCON2bits.ACKEN = 1; // initiate bus acknowledge sequence
    PIR1bits.SSPIF = 0;
    while (!PIR1bits.SSPIF); // wait until ACK sent
    INTCON = IE_stat;
}

/********************************************************************
*     Function Name:    StopI2C                                     *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      Send I2C bus stop condition.                *
********************************************************************/
//#undef StopI2C

void StopI2C(void) {
    char IE_stat;

    IE_stat = INTCON;
    INTCONbits.GIE = 0;
    SSPCON2bits.PEN = 1; // initiate bus stop condition
    PIR1bits.SSPIF = 0;
    while (!PIR1bits.SSPIF); // wait until Stop sent
    INTCON = IE_stat;
}

/********************************************************************
*     Function Name:    CloseI2C                                   *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      This function disables the SSP1 module. Pin *
*                       I/O returns under the control of the port   *
*                       registers.                                  *
********************************************************************/
//#undef CloseI2C
void CloseI2C( void )
{
  SSPCON1 &= 0xDF;                // disable synchronous serial port
}

/********************************************************************
*     Function Name:    NotAckI2C                                   *
*     Return Value:     void                                        *
*     Parameters:       void                                        *
*     Description:      Initiate NOT ACK bus condition.             *
********************************************************************/
//#undef Not_AckI2C

void Not_AckI2C(void) {
    char IE_stat;

    IE_stat = INTCON;
    INTCONbits.GIE = 0;
    SSPCON2bits.ACKDT = 1; // set acknowledge bit for not ACK
    SSPCON2bits.ACKEN = 1; // initiate bus acknowledge sequence
    PIR1bits.SSPIF = 0;
    while (!PIR1bits.SSPIF); // wait until Stop sent
    INTCON = IE_stat;
}

/********************************************************************
*     Function Name:    WriteStr_I2C(Mod_Addr, Reg_Addr, *Str, Str_Size)*
*     Return Value:     char Error 0=OK                                       *
*     Parameters:       void                                        *
*     Description:      Write a STring to I2C Bus            *
********************************************************************/

char WriteStr_I2C(char Mod_Addr, char Reg_Addr, char *Str, char Str_Size){
    
    char stat=0,i=0;

// Open I2C BUs as Master
    
    OpenI2C(MASTER, SLEW_OFF);
    SSPADD=0x18;
    IdleI2C();

// Send Module Address    
    StartI2C();
    stat=WriteI2C(Mod_Addr);
    stat+=Wait_AckI2C();
  
// Point to Register Address to write    
    stat+=WriteI2C(Reg_Addr);
    stat+=Wait_AckI2C();
 
//Send string to I2C Bus    
    while(Str_Size--){
        stat+=WriteI2C(*Str++);
        stat+=Wait_AckI2C();
    }        
    StopI2C();
    CloseI2C();
    return (stat);
    
}  

/********************************************************************
*     Function Name:    ReadStr_I2C(Mod_Addr, Reg_Addr, *Str, Str_Size)*
*     Return Value:     char Error 0=OK                                       *
*     Parameters:       void                                        *
*     Description:      Read a sring from I2C Bus            *
********************************************************************/

char ReadStr_I2C(char Mod_Addr, char Reg_Addr, char *Str, char Str_Size){
    
    char stat=0;
 
    //OPen I2C as Master
    OpenI2C(MASTER, SLEW_OFF);
    SSPADD=0x18;
    IdleI2C();
    
    // Send Module address
    StartI2C();
    stat=WriteI2C(Mod_Addr);
    if(stat!=0){
        StopI2C();
        CloseI2C();
        return(0xff);
    }
    
    stat=Wait_AckI2C();
    if(stat!=0){
        StopI2C();
        CloseI2C();
        return(0xff);
    }
    
    //Send Register address
    stat=WriteI2C(Reg_Addr);
    if(stat!=0){
        StopI2C();
        CloseI2C();
        return(0xff);
    }
    stat=Wait_AckI2C();
    if(stat!=0){
        StopI2C();
        CloseI2C();
        return(0xff);
    }
    
    RestartI2C();
    
    //Send Register address
    stat=WriteI2C(Mod_Addr|0x01);
    if(stat!=0){
        StopI2C();
        CloseI2C();
        return(0xff);
    }
    stat=Wait_AckI2C();
    if(stat!=0){
        StopI2C();
        CloseI2C();
        return(0xff);
    }
    
    while(Str_Size--){
            *Str++=ReadI2C();
            if(Str_Size!=0)
                AckI2C();
            else
                Not_AckI2C();
        }
    
    StopI2C();
    CloseI2C();
    return (0x00);
    
}