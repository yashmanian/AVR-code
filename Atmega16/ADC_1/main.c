#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "lcd.h"
char ADC_result [4];	//LCD string variable declaration

int main (void)
{
	LCDInit(LS_ULINE);   //initialise LCD
	
	//Configure ADC
	ADCSRA |= 1 << ADPS2 |1 << ADPS1 | 1 << ADPS0;	//Enable pre-scaler : Enabled by clock pulse ( range= 1600/5=320 to 160/2=80)
	ADMUX |= 1 << ADLAR;	//8-bit or 10-bit results
	ADMUX |= 1 << REFS0;	//Deciding reference voltage
	ADCSRA |= 1 << ADIE;	//Enable interrupts for ADC
	sei();	//Declare global interrupts
	ADCSRA |= 1 << ADEN;	//Turn on the ADC
	
	//Start first conversion
	ADCSRA |= 1 << ADSC;
	
	LCDWriteStringXY(0,0,"ADC_1");
	while(1)
	{
	}
}


ISR (ADC_vect)
{
	itoa ( ADCH,ADC_result,10);	//Convert ADC result to string
	LCDGotoXY(1,1);		//Select location on LCD to display variable
	LCDWriteStringXY(0,1,ADC_result);	//Go to location and display string on LCD
	LCDWriteString(" ");
	ADCSRA |= 1 << ADSC;	//Start next conversion
	_delay_ms(80);
}