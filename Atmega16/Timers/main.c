#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include<avr/io.h>


int main (void)
{
	//initialising LEDs
	DDRD |= 1 << PIND6;
	PORTD &= ~(1 << PIND6);
	TCCR1B |= 1 << CS10 | 1 << CS12;
	
	
	while(1)
	{
		//count and toggle LED
		if (TCNT1 > 15625)
		{
			TCNT1 = 0;
			PORTD ^= 1 << PIND6;	
		}	
	}
}