#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "global.h"
#include "SOR_Utils.h"


void initpwm()
{
		TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS00);//|(1<<CS00);

		DDRB|=(1<<PINB3); // set output for PWM timer 0
		
		TCCR1A|=(1<<WGM10)|(1<<COM1A1);//|(1<<COM1A0);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
      
		DDRD|=(1<<PIND5); // set output for PWM timer 1
		
		TCCR2|=(1<<WGM21)|(1<<WGM20)|(1<<COM21)|(1<<CS20);//|(1<<COM20);

		DDRD|=(1<<PIND7); // set putput for PWM timer 2
}




void setpwmoutput1(int8_t duty1,int8_t duty2,int8_t duty3)
{
   OCR0 = duty1;
   OCR1A = duty2;
   OCR2 = duty3;
}

int main()
{	
	DDRB|= 0b11111111;
	//DDRA|= 0b00000011;
	
	//int8_t m1=0;
	//int8_t m2=0;
	//int8_t m3=0;
	
	
	initpwm();
	
			uartInit();
			uartSetBaudRate(9600);
			rprintfInit(uartSendByte);
	
		TCNT0=0;
	    TCNT1=0;
	    TCNT2=0;
		PORTB|= 0b00000000;
		OCR0=0;
		PORTD|= 0b00000000;
	
   //Do this forever

   while(1)
   {
		initpwm();
		char data=uartGetByte();
		
		if(data == 'w')
		{
			rprintf("forward\n");
			rprintf("%c w \n", data);
			PORTB|= (1<<PINB0)|(1<<PINB7); //(1<<PINB5)|
			setpwmoutput1(255,0,255);
			_delay_ms(15);
		}
		else if(data == 's')
		{
			rprintf("reverse\n");
			rprintf("%c s \n", data);
			PORTB|= (1<<PINB1)|(1<<PINB6); //|(1<<PINB4)
			setpwmoutput1(255,0,255);
			_delay_ms(15);
		}
		else if(data == 'a')
		{
			rprintf("left\n");
			rprintf("%c a \n", data);
			PORTB|= (1<<PINB4)|(1<<PINB7); //(1<<PINB0)|
			setpwmoutput1(0,255,255);
			_delay_ms(15);
			
		}
		else if(data == 'd')
		{
			rprintf("right\n");
			rprintf("%c d \n", data);
			PORTB|= (1<<PINB5)|(1<<PINB6); //(1<<PINB0)|
			setpwmoutput1(0,255,255);
			_delay_ms(15);
		}
		else
		{
			setpwmoutput1(0,0,0);
			PORTB|= 0b00000000;
			PORTD|= 0b00000000;
		}
	   
	}

}







