#include <LPC214x.H>

#include "lcd_ctrl.h"
#include "tools.h"

#define __DELAY	   20

void lcd_command(char type, unsigned char x) //type: 0-instruction; 1-data
{
    IO1PIN = ( (IO1PIN & 0xFC00FFFF) | x<<16 | type<<24 | 1<<25);
    delay(__DELAY);
    IO1CLR = 1<<25;
    delay(__DELAY);
}

void lcd_s(unsigned char addr, char *s) 
{
	//afisare string s; addr = pozitia de unde sa inceapa: linia 1: 0...15; linia 2: 64...79
    unsigned char i=0;
    lcd_command(0, 0x06); 
    lcd_command(0, (1<<7) | addr );
    
    while(s[i]!='\0')
        lcd_command(1, s[i++]);   
}

void lcd_n10(unsigned char addr, int nr, char poz, char leading_zero)
{
	//afisare nr in baza 10; poz = pe cate caractere sa afiseze nr;
	//		daca nr are mai putine cifre decat valoarea poz, se completeaza cu zerouri daca leading_zero=1, altfel cu spatii;
    unsigned char i=0;          
    lcd_command(0, 0x04);
    lcd_command(0, (1<<7) | (addr+poz-1) );
    
    do
    {
        lcd_command(1, nr%10 + '0');
        nr/=10;
        i++;
    } while(nr!=0);
    
    while(i++<poz)
		if(leading_zero)
        	lcd_command(1, '0');
		else
			lcd_command(1, ' ');
}
