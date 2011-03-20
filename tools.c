#include "tools.h"

void delay(int nr)
{
	int i;
	for( ;nr>0;nr--)
		for(i=0;i<1000;i++);
}
