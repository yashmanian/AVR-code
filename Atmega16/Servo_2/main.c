#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main (void)
{
	DDRD |= 0xFF;
	TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0;
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10 | 1<<CS12;
	ICR1 = 19999;
	
	while(1)
	{
		OCR1A = ICR1-800;
		_delay_ms(100);
		OCR1A = ICR1-800;
		_delay_ms(100);
	}
}

