/* PIC18 I2C peripheral library header */

/* SSPCON1 REGISTER */
#define   SSPENB    0x20  /* Enable serial port and configures
                             SCK, SDO, SDI                      */
#define   SLAVE_7   6     /* I2C Slave mode, 7-bit address      */
#define   SLAVE_10  7     /* I2C Slave mode, 10-bit address     */
#define   MASTER    8     /* I2C Master mode                    */

/* SSPSTAT REGISTER */
#define   SLEW_OFF  0xC0  /* Slew rate disabled for 100kHz mode */
#define   SLEW_ON   0x00  /* Slew rate enabled for 400kHz mode  */


/* FUNCTION PROTOTYPES */


void OpenI2C( unsigned char, unsigned char);
void IdleI2C(void);
void StartI2C(void);
unsigned char WriteI2C(unsigned char);
char Wait_AckI2C(void);
void RestartI2C(void);
unsigned char ReadI2C(void);
void AckI2C(void);
void StopI2C(void);
void Not_AckI2C(void);
void CloseI2C(void);
char WriteStr_I2C(char, char, char *, char);
char ReadStr_I2C(char, char, char *, char);

