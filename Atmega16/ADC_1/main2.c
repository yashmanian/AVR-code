#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "lcd.h"

int main (void)
{
	LCDInit(LS_ULINE);   //initialise LCD
	LCDWriteStringXY(0,0,"ADC_1");
	
	ADCSRA |= 1<<ADPS2;
	ADMUX |= (1<<REFS0) | (1<<REFS1);
	ADCSRA |= 1<<ADIE;
	ADCSRA |= 1<<ADEN;

	sei();

	ADCSRA |= 1<<ADSC;
	
	while(1)
	{
	}
}

ISR (ADC_vect)
{
	uint8_t theLowADC = ADCL;
	uint16_t theTenBitResults = ADCH<<8 | theLowADC;
	LCDWriteIntXY(0,1,theTenBitResults,4)
	ADCSRA |= 1 << ADSC;	//Start next conversion
	_delay_ms(80);
}