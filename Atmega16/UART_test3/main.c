#define F_CPU 16000000UL
#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>


//This function is used to initialize the USART
//at a given UBRR value
void USARTInit(uint16_t ubrr_value)
{

   //Set Baud rate

   UBRRL = ubrr_value;
   UBRRH = (ubrr_value>>8);

   /*Set Frame Format

            
   >> Asynchronous mode
   >> No Parity
   >> 1 StopBit

   >> char size 8

   */

   UCSRC=(1<<URSEL)|(3<<UCSZ0);


   //Enable The receiver and transmitter

   UCSRB=(1<<RXEN)|(1<<TXEN)|(1<<RXCIE);


}


//This function is used to read the available data
//from USART. This function will wait untill data is
//available.
char USARTReadChar()
{
   //Wait untill a data is available

   while(!(UCSRA & (1<<RXC)))
   {
      //Do nothing
   }

   //Now USART has got data from host
   //and is available is buffer

   return UDR;
}


//This fuction writes the given "data" to
//the USART which then transmit it via TX line
void USARTWriteChar(char data)
{
   //Wait untill the transmitter is ready

   while(!(UCSRA & (1<<UDRE)))
   {
      //Do nothing
   }

   //Now write the data to USART buffer

   UDR=data;
}

void main()
{
	char data;
	DDRD|= 1<<PIND6;
	USARTInit(51);
	while(1)
	{
	
	} 
}

ISR(USART_RXC_vect)
{
	char data;
	data=USARTReadChar();
	USARTWriteChar(data);
	if(data == 'a')
	{
		PORTD|= 1<<PIND6;
	}
	else
	{
		PORTD&=~(1<<PIND6);
	}
}