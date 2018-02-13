#define F_CPU 16000000UL

#include<avr/io.h>
#include<avr/interrupt.h>


int main (void)   //main routine
{
    sei();   //initialise global interrupts
	DDRD |= 1 << PIND6;
	TCCR1B |= 1 << CS10 | 1 << CS12 | 1 << WGM12;   //using prescaler of 1024 (CS10 and CS12 pulled high) and enabling Waveform generation mode (WGM12)
	TIMSK |= 1 << OCIE1A;   //Output Compare Interrupt Enable 1(16-bit) A(register A)
	OCR1A = 15625;
	
	
	while(1)
	{
	}
}

ISR(TIMER1_COMPA_vector)   //interrupt service routine (timer interrupt setup)
{
	PORTD ^= 1 << PIND6;
}