// Definiciones

#define  Serial_Data LATCbits.LC0		
#define  Shift_Reg_Clock LATCbits.LC1
#define  Reg_Clock LATCbits.LC2
#define  Out_Enable LATCbits.LC5


void WriteIO5R (unsigned long int);
void OpenIO5R(void);
char Rd_Digital_Inputs(void); 
void OpenADC(void);
unsigned int Get_Analog_Channel(char);

union Data_out
{
	unsigned long int Output;
	char bytes[4];
    struct{
        char No_Use;
        char Relays;
        char LedBlock2;
        char LedBlock1;
    };
	struct{
		unsigned fill:8;
		unsigned rly1:1;
		unsigned rly2:1;
		unsigned rly3:1;
		unsigned rly4:1;
		unsigned rly5:1;
		unsigned f1:3;
		unsigned led9:1;
		unsigned led10:1;
		unsigned led11:1;
		unsigned led12:1;
		unsigned led13:1;
		unsigned led14:1;
		unsigned led15:1;
		unsigned led16:1;
		unsigned led1:1;
		unsigned led2:1;
		unsigned led3:1;
		unsigned led4:1;
		unsigned led5:1;
		unsigned led6:1;
		unsigned led7:1;
		unsigned led8:1;
	};	
};	

union Digital_Inputs
{
	unsigned char byte;
	struct {
		unsigned B0:1;
		unsigned B1:1;
		unsigned B2:1;
		unsigned B3:1;
		unsigned B4:1;
		unsigned B5:1;
		unsigned B6:1;
		unsigned B7:1;
	};
};	

struct Analog_Inputs {
    unsigned int ADC;
    float In_Volt;
};


