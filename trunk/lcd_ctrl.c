#include <LPC214x.H>

#include "lcd_ctrl.h"
#include "tools.h"

void lcd_command(unsigned char x)
{
    IO1PIN = (IO1PIN & 0xFC00FFFF) | x<<16 | 1<<25;	// put data and E=1
    delay_us(1);
    IO1CLR = 1<<25;	//E=1
}

void lcd_init(void)
{
	delay_ms(30);
	lcd_command(0x38); 
	delay_us(50);
    lcd_command(0x0C);
	delay_us(50);
	lcd_command(0x01);
	delay_ms(2);
}

void lcd_putchar(unsigned char x)
{
    IO1PIN = ( (IO1PIN & 0xFC00FFFF) | x<<16 | 1<<24 | 1<<25);
    delay_us(1);
    IO1CLR = 1<<25;
	delay_us(50);
}

void lcd_puts(unsigned char addr, char *s) 
{
	//afisare string s; addr = pozitia de unde sa inceapa: linia 1: 0...15; linia 2: 64...79
    unsigned char i=0;
    lcd_command(0x06); //increment mode
	delay_us(50); 
    lcd_command( (1<<7) | addr );
	delay_us(50);
    
    while(s[i]!='\0')
        lcd_putchar(s[i++]);   
}

void lcd_putnum(unsigned char addr, int nr, char length, char leading_zero)
{
	//afisare nr in baza 10; length = pe cate caractere sa afiseze nr;
	//		daca nr are mai putine cifre decat valoarea length, se completeaza cu zerouri daca leading_zero=1, altfel cu spatii;
    unsigned char i=0;          
    lcd_command(0x04); //decrement mode
	delay_us(50);
    lcd_command( (1<<7) | (addr+length-1) );
	delay_us(50);
    
    do
    {
        lcd_putchar(nr%10 + '0');
        nr/=10;
        i++;
    } while( (nr!=0) && (i<length) );
    
    while(i++<length)
		lcd_putchar(leading_zero ? '0' : ' ');
}
