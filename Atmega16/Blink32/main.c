#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
 int main(void)
 {
	DDRD|=1<<PIND6;
	while(1)
	{
		PORTD^= 1<<PIND6;
		_delay_ms(1000);
	}
 }