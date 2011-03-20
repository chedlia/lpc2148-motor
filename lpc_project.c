/*
	LPC2148 Project
	Stepper Motor Control
*/

#include <LPC214x.H>

#include "lcd_ctrl.h"
#include "tools.h"

int main(void)
{
	int i=0, adc_value=123, dir=1;
	int step[] = { 0x00000000, 0x00200000, 0x00201000, 0x00001000 };

	IODIR0 = 0x40601000;
	IODIR1 = 0x03FF0000;

	IO0SET = 1<<30;

	lcd_command(0, 0x0C);
    lcd_command(0, 0x38);
    
    lcd_s(64,"ADC: ");
    lcd_n10(69,0,4,0);
	lcd_n10(0, sizeof(char), 2, 1);
	lcd_n10(3, sizeof(short int), 2, 1);
	lcd_n10(6, sizeof(long int), 2, 1);
	lcd_s(11, "UDLRC");
	
	AD0CR = 0x01200806;

 	while(1)
	{
		//if( ((AD0GDR>>31)&1) == 1)
		if ( (AD0GDR & (1 << 31)) != 0)
		{
			adc_value = (AD0GDR>>6)& 0x3FF;
			AD0CR = 0x01200806;
		}

		lcd_n10(69, adc_value, 4, 0);
		delay(100);
		lcd_n10(75, IOPIN0>>17&1, 1, 0); 
		lcd_n10(76, IOPIN0>>20&1, 1, 0);
		lcd_n10(77, IOPIN0>>19&1, 1, 0);
		lcd_n10(78, IOPIN0>>18&1, 1, 0);
		lcd_n10(79, IOPIN0>>16&1, 1, 0);

		IO0PIN = (IO0PIN & 0xFFDFEFFF) | step[i];
		delay(1023-adc_value);
		
		dir = IO0PIN>>16&1;
		if(dir)
		{
			i++;
			if(i == 4)
				i = 0;
		}
		else
		{
			i--;
			if(i == -1)
				i = 3;
		}	
		
		/*
		IO0SET = 1<<12;
		delay();
		IOSET0 = 1<<21;
		delay();
		IOCLR0 = 1<<12;
		delay();
		IOCLR0 = 1<<21;
		delay();
		*/
	}
}
