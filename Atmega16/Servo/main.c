#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main (void)
{
	DDRD |= 0xFF;
	TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0;
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10;
	ICR1 = 19999;
	uint8_t servo_pos;
	while(1)
	{
		for(servo_pos=0;servo_pos<19;servo_pos++)
		{
			OCR1B = ICR1 - (servo_pos*1000);
			_delay_ms(100);
		}
	}
}

