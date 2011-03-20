/*
	LCD Controller Library
*/

#ifndef LIB_LCDCTRL
#define LIB_LCDCTRL

void lcd_command(char type, unsigned char x);
void lcd_s(unsigned char addr, char *s);
void lcd_n10(unsigned char addr, int nr, char poz, char leading_zero);

#endif
