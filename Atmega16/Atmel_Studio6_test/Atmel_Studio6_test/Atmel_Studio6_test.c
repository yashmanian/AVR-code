/*
 * Atmel_Studio6_test.c
 *
 * Created: 06-07-2014 09:04:59
 *  Author: Yash Manian
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	DDRD|=1<<PIND6;
    while(1)
    {
		PORTD^=1<<PIND6;
        _delay_ms(800);//TODO:: Please write your application code 
    }
}