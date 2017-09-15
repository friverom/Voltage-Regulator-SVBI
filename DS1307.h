/* Definiciones para el manejo del DS1307 */

#include <time.h>

#define MOD_ADDR 0xd0
#define TIME_REG 0x00
#define DATE_REG 0x03


struct Time_Data {
	unsigned char seconds;
	unsigned char minutes;
	unsigned char hours;
};

struct Date_Data {
	unsigned char day_w;
	unsigned char date;
	unsigned char month;
	unsigned char year;
};

char get_time(char *);
char get_date(char *);
char set_time(char *);
char set_date(char *);
int bcdTOint(int bcdByte);
int intTObcd(int);
time_t time (time_t * t);	