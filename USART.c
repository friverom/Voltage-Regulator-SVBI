/*
 * File:   USART.c
 * Author: Federico Rivero
 *
 * Created on July 21, 2016, 1:28 PM
 */

#include <xc.h>
#include <stdio.h>

 #define USART 1



void OpenUSART(unsigned char config, unsigned spbrg) {
    TXSTA = 0; // Reset USART registers to POR state
    RCSTA = 0;
    TRISCbits.RC6=0;
    TRISCbits.RC7=1;

    if (config & 0x01) // Sync or async operation
        TXSTAbits.SYNC = 1;

    if (config & 0x02) // 8- or 9-bit mode
    {
        TXSTAbits.TX9 = 1;
        RCSTAbits.RX9 = 1;
    }

    if (config & 0x04) // Master or Slave (sync only)
        TXSTAbits.CSRC = 1;

    if (config & 0x08) // Continuous or single reception
        RCSTAbits.CREN = 1;
    else
        RCSTAbits.SREN = 1;

    if (config & 0x10) // Baud rate select (asychronous mode only)
        TXSTAbits.BRGH = 1;
    else
        TXSTAbits.BRGH = 0;

    PIR1bits.TXIF = 0;

    if (config & 0x40) // Interrupt on receipt
        PIE1bits.RCIE = 1;
    else
        PIE1bits.RCIE = 0;

    //PIR1bits.RCIF = 0;

    if (config & 0x80) // Interrupt on transmission
        PIE1bits.TXIE = 1;
    else
        PIE1bits.TXIE = 0;

    SPBRG = spbrg; // Write baudrate to SPBRG1

    TXSTAbits.TXEN = 1; // Enable transmitter
    RCSTAbits.SPEN = 1; // Enable receiver
}


#ifdef USART
void putch(char byte) {

    while (!PIR1bits.TXIF) //Esperar a TXREG este disponible para TX
        continue;
    TXREG = byte; //Enviar caracter
}

char getchar() {
    while (!PIR1bits.RCIF)
        continue;
    return (RCREG);
}

#endif

