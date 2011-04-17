/*
	Miscellaneous Function Library
*/

#ifndef LIB_MISC
#define LIB_MISC

#define FOR_CYCLES_1US 12

void delay_us(int nr);
void delay_ms(int nr);
char is_pressed(int port, char pin, char *flag);

#endif
