

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
	DDRD|=1<<PIND6;
	DDRB|= (1<<PINB3)|(1<<PINB4)|(1<<PINB5);
	while(1)
	{
		char data=uartGetByte();
		
		if(data == 'w')
		{
			rprintf("%c\n",data);
			PORTB|=1<<PINB3;
			PORTB&=~(1<<PINB4);
			_delay_ms(200);
			PORTB&=~(1<<PINB4);
			PORTB&=~(1<<PINB3);
			PORTB|=1<<PINB5;
			_delay_ms(5);
			PORTB&=~(1<<PINB5);
			rprintf("Complete\n");
			
		}
		
	}
}
	