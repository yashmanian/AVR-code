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
	while(1)
	{
		
		char data=uartGetByte();
		
		if(data == 'w')
		{
			rprintf("forward\n");
			rprintf("%c\n\r",data);
			_delay_ms(20);
		}
		else if(data == 's')
		{
			rprintf("reverse\n");
			rprintf("%c\n\r",data);
			_delay_ms(20);		
		}
		else if(data == 'a')
		{
			rprintf("left\n");
			rprintf("%c\n\r",data);
			_delay_ms(20);
			
		}
		else if(data == 'd')
		{
			rprintf("right\n");
			rprintf("%c\n\r",data);
			_delay_ms(20);
		}
		else
		{
			PORTB=0b00000000;
		}
	}
}
	