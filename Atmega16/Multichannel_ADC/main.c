#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
int main(void)
{
	LCDInit(LS_ULINE);
	LCDWriteStringXY(0,0,"sensor1:");
	LCDWriteStringXY(0,1,"sensor2:");

	ADCSRA |= 1<<ADPS2| (1<<ADPS1) | (1<<ADPS0);
	ADMUX |= 1<<REFS0 | 1<<REFS1;
	ADCSRA |= 1<<ADIE;
	ADCSRA |= 1<<ADEN;

	sei();

	ADCSRA |= 1<<ADSC;

	while (1)
	{
	}
}
ISR(ADC_vect)
{
	uint8_t theLow = ADCL;
	uint16_t theTenBitResult = ADCH<<8 | theLow;

	switch (ADMUX)
	{
		case 0xC0:
		LCDWriteIntXY(9, 0, theTenBitResult, 4);
		ADMUX = 0xC1;
		break;
		case 0xC1:
		LCDWriteIntXY(9, 1, theTenBitResult, 4);
		ADMUX = 0xC0;
		break;
		default:
		//Default code
		break;
	} 
	ADCSRA |= 1<<ADSC;
	_delay_ms(80);
}