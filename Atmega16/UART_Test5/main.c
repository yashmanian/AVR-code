#define F_CPU 16000000UL
#include <avr/io.h>
#include "global.h"
#include <util/delay.h>
#include "SOR_Utils.h"

int main()
{
	uartInit();
	uartSetBaudRate(9600);
	rprintfInit(uartSendByte);
	DDRB=0b11111111;
	while(1)
	{
		
		char data=uartGetByte();
		
		if(data == 'w')
		{
			rprintf("forward\n");
			rprintf("%c\n\r",data);
			PORTB|= (1<<PINB4)|(1<<PINB7);
			_delay_ms(20);
		}
		else if(data == 's')
		{
			rprintf("reverse\n");
			rprintf("%c\n\r",data);
			PORTB|= (1<<PINB5)|(1<<PINB6);
			_delay_ms(20);		
		}
		else if(data == 'a')
		{
			rprintf("left\n");
			rprintf("%c\n\r",data);
			PORTB|= (1<<PINB1)|(1<<PINB6);
			_delay_ms(20);
			
		}
		else if(data == 'd')
		{
			rprintf("right\n");
			rprintf("%c\n\r",data);
			PORTB|= (1<<PINB0)|(1<<PINB7);
			_delay_ms(20);
		}
		else
		{
			PORTB=0b00000000;
		}
	}
}
	