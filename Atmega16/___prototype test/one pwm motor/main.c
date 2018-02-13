#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "global.h"
#include "SOR_Utils.h"


void initpwm1()
{
		TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01);//|(1<<CS00);

		DDRB|=(1<<PINB3);
}
void setpwmoutput1(int8_t duty1)
{
   OCR0=duty1;
}
void main()
{	

	initpwm1();
	uartInit();
	uartSetBaudRate(9600);
	rprintfInit(uartSendByte);
	
	
   //Do this forever

   while(1)
   {
		char data;
		
		data = uartGetByte;
       
	   _delay_us(220);
	   
	   {
	   initpwm1();
	   setpwmoutput1(84);
	   /*initpwm2();
	   setpwmoutput2(84);
	   initpwm3();
	   setpwmoutput3(-84);
		rprintf("%c", data);*/
		_delay_us(220);
		}
	}	
}	