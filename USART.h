#define USART_TX_INT_ON   0b11111111  // Transmit interrupt on
#define USART_TX_INT_OFF  0b01111111  // Transmit interrupt off
#define USART_RX_INT_ON   0b11111111  // Receive interrupt on
#define USART_RX_INT_OFF  0b10111111  // Receive interrupt off
#define USART_BRGH_HIGH   0b11111111  // High baud rate
#define USART_BRGH_LOW    0b11101111  // Low baud rate
#define USART_CONT_RX     0b11111111  // Continuous reception
#define USART_SINGLE_RX   0b11110111  // Single reception
#define USART_SYNC_MASTER 0b11111111  // Synchrounous master mode
#define USART_SYNC_SLAVE  0b11111011  // Synchrounous slave mode
#define USART_NINE_BIT    0b11111111  // 9-bit data
#define USART_EIGHT_BIT   0b11111101  // 8-bit data
#define USART_SYNCH_MODE  0b11111111  // Synchronous mode
#define USART_ASYNCH_MODE 0b11111110  // Asynchronous mode



void OpenUSART( unsigned char, unsigned);
void putch (char);
char getchar (void);