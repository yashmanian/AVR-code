
#define F_CPU 16000000UL

#include<avr/io.h>
#include<util/delay.h>

void main(void)
{
	DDRD|= 1<<PIND6;
	while(1)
	{
		PORTD ^= 1 << PIND6;
		_delay_ms(200);
	}
}