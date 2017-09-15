#define ON       0b11111111
#define OFF      0b01111111
#define MODE_8_BIT     0b11111111
#define MODE_16_BIT    0b10111111
#define EXT_CLK  0b11111111
#define INT_CLK  0b11011111
#define INC_HL   0b11111111
#define INC_LH   0b11101111
#define PRE_OFF   0b11111111
#define PRE_ON      0b11110111
#define PRE_256         0b11111111
#define PRE_128         0b11111110
#define PRE_64          0b11111101
#define PRE_32          0b11111100
#define PRE_16          0b11111011
#define PRE_8           0b11111010
#define PRE_4           0b11111001
#define PRE_2           0b11111000
#define INT_EN   1
#define INT_DIS  0

void Open_Timer0(char, char, int);

typedef union tim {
        unsigned int tim_value;
        struct {
        char HSB;
        char LSB;
        };
    }SplitInt;
