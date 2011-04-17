#include "tools.h"

void delay_us(int nr)
{
	for(nr*=FOR_CYCLES_1US; nr!=0; nr--);
}

void delay_ms(int nr)
{
	for(nr*=FOR_CYCLES_1US*1000; nr!=0; nr--);
}

char is_pressed(int port, char pin, char *flag)
{
	char ret = 0;
	if( (port & (1<<pin) ) == 0 )
	{
		if(*flag == 0)
		{
			*flag = 1;
			//delay_ms(20); //debouncing
			ret = 1; //when button is pressed (immediately after falling edge)
		}
		else
		{
			//count ...
			//ret = 2; //when button is hold
		}
	}
	else
	{
		if(*flag == 1)
		{
			*flag = 0;
			//delay_ms(20);	//debouncing
			//ret = 3; //when button is released (immediately after rising edge)
		}
		else
		{
			//button is not pressed
		}
	}
	return ret;
}
