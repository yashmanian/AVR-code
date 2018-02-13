#define F_CPU 16000000UL
#include <avr/io.h>
#include "global.h"
#include <util/delay.h>
#include "SOR_Utils.h"

	void pwminit()
	{
		TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS00);//|(1<<CS00);
		
		DDRB|=(1<<PINB3); // set output for PWM timer 0
		
		TCCR1A|=(1<<WGM10)|(1<<COM1A1);//|(1<<COM1A0);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
      
		DDRD|=(1<<PIND5); // set output for PWM timer 1
		
		TCCR2|=(1<<WGM21)|(1<<WGM20)|(1<<COM21)|(1<<CS20);//|(1<<COM20);
		
		DDRD|=(1<<PIND7); // set output for PWM timer 2
	}


	void setpwmoutput(int8_t duty1,int8_t duty2,int8_t duty3)
	{
		OCR0 = duty1;
		OCR1A = duty2;
		OCR2 = duty3;	
	}
	
	

int main()
{
	//pwminit();
	
	uartInit();
	uartSetBaudRate(9600);
	rprintfInit(uartSendByte);
	DDRB|=0b11111111;
	while(1)
	{
		pwminit();
		
		char data=uartGetByte();
		
		if(data == 's')
		{
			setpwmoutput(168,84,84);
			rprintf("reverse\n");
			rprintf("%c\n\r",data);
			PORTB|= (1<<PINB1)|(1<<PINB4)|(1<<PINB6)|(1<<PINB3);   //pin1,4,6 for direction & pin3,5,7 for pwm
			PORTD|= (1<<PIND5)|(1<<PIND7);
			_delay_ms(15);
  		    
		}
		else if(data == 'w')
		{
			setpwmoutput(168,84,84);
			rprintf("forward\n");
			rprintf("%c\n\r",data);
		PORTB|= (1<<PINB0)|(1<<PINB5)|(1<<PINB6)|(1<<PINB3);//pin1,4,6 for direction & pin3,5,7 for pwm
			PORTD|= (1<<PIND5)|(1<<PIND7);
			_delay_ms(15);		
		}
		else if(data == 'a')
		{
			setpwmoutput(0,147,147);
			rprintf("left\n");
			rprintf("%c\n\r",data);
			PORTB|= (1<<PINB4)|(1<<PINB7);//pin1,4,6 for direction & pin3,5,7 for pwm
			PORTD|= (1<<PIND5)|(1<<PIND7);
			_delay_ms(15);
			
		}
		else if(data == 'd')
		{
			setpwmoutput(0,147,147);
			rprintf("right\n");
			rprintf("%c\n\r",data);
			PORTB|= (1<<PINB5)|(1<<PINB6);//pin1,4,6 for direction & pin3,5,7 for pwm
			PORTD|= (1<<PIND5)|(1<<PIND7);
			_delay_ms(15);
		}
		else
		{
			PORTB=0b00000000;
		}
	}
}
	