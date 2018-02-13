#include <avr/io.h>
#include <YMlib/UART.h>
#include <util/delay.h>
char data;


void main()
{
	DDRD|=1<<PIND6;
	
		while(1)
	{
		data=USART_ReceiveByte();
		
	}
}

ISR(USART_RXC_vect)
{
	USART_TransmitByte(data);
	PORTD^=1<<PIND6;
}
