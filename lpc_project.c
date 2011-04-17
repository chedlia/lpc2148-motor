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

#define JC 16	//Joystick Center P0.16
#define JU 17	//...
#define JR 18
#define JL 19
#define JD 20
#define BT 14	//Push button P0.14

volatile int counter=0;	// count interrupts (for testing)
int adc_value; 
volatile char dir = 1; //motor direction: 1-ClockWise 0-CounterCW
int stepper_code[] = { 0x00000000, 0x00200000, 0x00201000, 0x00001000 };
volatile int stepper_pos;
char jl_flag, jr_flag, ju_flag, jd_flag, jc_flag, bt_flag;

void timer0matchISR(void) __irq
{
	IO0PIN = (IO0PIN & 0xFFDFEFFF) | stepper_code[stepper_pos];
		
	if(dir)
	{
		if (++stepper_pos == 4)
			stepper_pos = 0;
	}
	else
	{
		if (--stepper_pos == -1)
			stepper_pos = 3;
	}
	T0MR0 = 3*15000000UL/adc_value;	// --------------------problem if adc_value is too small
	//counter++;
	T0IR = 0x01; //clear interrupt flag
	VICVectAddr = 0; //end of interrupt - update priority hardware
}
                       					
int main(void)
{
	//int i=0;
	int adc_temp;

	IO0DIR = 0x40601000;
	IO1DIR = 0x03FF0000; 
	VPBDIV = 0;  // 0 -> PCLK=1/4*CCLK; 1 -> PCLK=CCLK; 2 -> PCLK=1/2*CCLK  (APBDIV in User manual)

	IO0SET = 1<<30; //for LCD backlight power

	T0CTCR = 0x0; //Count control register: Timer mode
	T0PR = 0; //Prescale register
	//T0MR0 = 15000000UL; //Match register 0
	//T0MR0 = 3*15000000UL/60; //1 rot/s
	T0MR0 = 15000000UL; //interrupt at 1 second
	T0MCR = 0x3; //Match control register: Interrupt, Reset timer
	T0TCR = 0x1; //Timer enabled for counting

	VICVectCntl0 = 1<<5 | 4; //enable slot 0 interrupt, timer 0
	VICVectAddr0 = (unsigned int)timer0matchISR; //slot 0 ISR
	VICIntSelect = 0x00000000; //interrupt type 0=IRQ 1=FIQ
	VICIntEnable = 1<<4; //enable timer 0 interrupt

	lcd_init();
    
    lcd_puts(64,"ADC: ");
    lcd_putnum(69,0,4,0);

	lcd_puts(11, "UDLRC");
	
	//PCONP |= 1<<12;	//power for ADC
	PINSEL1 = 1<<26 ; //configure pin as ADC input
	AD0CR = 0x00211004;	  // PCLK=15MHz; ADCLK<=4.5MHz ; PCLK/ADCCLK=3.33 -> 4=CLKDIV+1 -> CLKDIV=3
	AD0CR |= 0x01000000; //----------------------------

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
			adc_temp = AD0GDR;
		}
		while( (adc_temp & 0x80000000) == 0);	//----------------------

		adc_value = (adc_temp >>6) & 0x03FF;
		
		lcd_putnum(69, adc_value, 4, 0);
		
		lcd_putnum(75, IOPIN0>>17&1, 1, 0); 
		lcd_putnum(76, IOPIN0>>20&1, 1, 0);
		lcd_putnum(77, IOPIN0>>19&1, 1, 0);
		lcd_putnum(78, IOPIN0>>18&1, 1, 0);
		lcd_putnum(79, IOPIN0>>16&1, 1, 0);

		//T0MR0 = 3*15000000UL/adc_value;

		if (is_pressed(IO0PIN, JL, &jl_flag))
			counter++;
		if (is_pressed(IO0PIN, JR, &jr_flag))
			counter++;
		if (is_pressed(IO0PIN, JU, &ju_flag))
			counter++;
		if (is_pressed(IO0PIN, JD, &jd_flag))
			counter++;
		if (is_pressed(IO0PIN, JC, &jc_flag))
		{
			counter++;
			dir = 1 - dir;
		}

		if (is_pressed(IO0PIN, BT, &bt_flag))
			counter++;
			
		lcd_putnum(0, counter, 4, 0);
		
		//delay_ms(10); 
/*
		IO0PIN = (IO0PIN & 0xFFDFEFFF) | step[i];
		delay_us((1023-adc_value)*60);	  //48,3 Hz max signal freq for stepper
		
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
*/
	}
}
