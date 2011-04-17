/*
	LCD Controller Library
*/

#ifndef LIB_LCDCTRL
#define LIB_LCDCTRL

void lcd_command(unsigned char x);
void lcd_init(void);
void lcd_putchar(unsigned char x);
void lcd_puts(unsigned char addr, char *s);
void lcd_putnum(unsigned char addr, int nr, char poz, char leading_zero);

#endif
