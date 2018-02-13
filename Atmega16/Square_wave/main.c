#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void main()
{
	DDRB|= 1<<PINB3;
	while(1)
	{
		PORTB^= 1<<PINB3;
		_delay_us(8);
	}
}