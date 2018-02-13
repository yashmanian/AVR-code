#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRD|= 1<<PIND4;
	while(1)
	{
		PORTD^= 1<<PIND4;
		_delay_ms(1000);
	}
}