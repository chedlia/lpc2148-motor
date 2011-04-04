/*
	LPC2148 Project
	Stepper Motor Control
*/

/*
	TortoiseSVN
	SlikSVN -- command line (bin to enviroment)

	svn checkout <url> <username>  (one time)
	svn update

	svn add tools.c

	svn status (modifcari locale)

	svn commit -m "Minor modifications" (upload)
*/

#include <LPC214x.H>

#include "lcd_ctrl.h"
#include "tools.h"

volatile int counter=0;	// count interrupts (for testing)

void timer0matchISR(void) __irq
{
	T0IR = 0x01; //clear interrupt flag
	counter++;
	//VICVectAddr = 0;
}

int main(void)
{
	int i=0, adc_value=123; 
	char dir = 1; //motor direction: 1-ClockWise 0-CounterCW
	int step[] = { 0x00000000, 0x00200000, 0x00201000, 0x00001000 };

	IO0DIR = 0x40601000;
	IO1DIR = 0x03FF0000; 

	IO0SET = 1<<30; //for LCD power

	T0CTCR = 0x0; //Count control register: Timer mode
	T0PR = 0; //Prescale register
	T0MR0 = 3000000; //Match register 0
	T0MCR = 0x3; //Match control register: Interrupt, Reset timer
	T0TCR = 0x1; //Timer enabled for counting

	VICVectCntl0 = 1<<5 | 4; //enable slot 0 interrupt, timer 0
	VICVectAddr0 = (unsigned)timer0matchISR; //slot 0 ISR
	VICIntSelect = 0x00000000; //interrupt type 0=IRQ 1=FIQ
	VICIntEnable = 1<<4; //enable timer 0 interrupt

	lcd_command(0, 0x0C);
    lcd_command(0, 0x38);
    
    lcd_s(64,"ADC: ");
    lcd_n10(69,0,4,0);

	lcd_s(11, "UDLRC");
	
	VPBDIV = 0;  // 0 -> PCLK=1/4*CCLK; 1 -> PCLK=CCLK; 2 -> PCLK=1/2*CCLK  (APBDIV in User manual)
	PCONP |= 1<<12;	//power for ADC
	PINSEL1 |= 1<<26; //configure pin as ADC input
	AD0CR = 0x00211004;
	AD0CR |= 0x01000000;

 	while(1)
	{
		/*if( ((AD0GDR>>31)&1) == 1)
		if ( (AD0GDR & (1 << 31)) != 0)
		{
			adc_value = (AD0GDR>>6)& 0x3FF;
			AD0CR = 0x01200806;
		} */

		do
		{
			adc_value = AD0GDR;
		}
		while( (adc_value & 0x80000000) == 0);

		adc_value = (adc_value >>6) & 0x03FF;
		
		lcd_n10(69, adc_value, 4, 0);
		
		lcd_n10(75, IOPIN0>>17&1, 1, 0); 
		lcd_n10(76, IOPIN0>>20&1, 1, 0);
		lcd_n10(77, IOPIN0>>19&1, 1, 0);
		lcd_n10(78, IOPIN0>>18&1, 1, 0);
		lcd_n10(79, IOPIN0>>16&1, 1, 0);

		lcd_n10(0, counter, 4, 0);

		IO0PIN = (IO0PIN & 0xFFDFEFFF) | step[i];
		delay((1023-adc_value)*4);
		
		dir = IO0PIN>>16 & 1;

		if(dir)
		{
			if (++i == 4)
				i = 0;
		}
		else
		{
			if (--i == -1)
				i = 3;
		}
	}
}

