#define F_CPU 16000000UL
#include <avr/io.h>
#include "global.h"
#include <util/delay.h>
#include "SOR_Utils.h"

	void pwminit()
	{
		TCCR1A|=(1<<WGM10)|(1<<COM1A1);//|(1<<COM1A0);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
      
		DDRD|=(1<<PIND5); // set output for PWM timer 1
		
		
	}


	void setpwmoutput(int8_t duty1)
	{
		OCR1A = duty1;
		
	}
	
	

int main()
{
	//pwminit();
	
	uartInit();
	uartSetBaudRate(9600);
	rprintfInit(uartSendByte);
	DDRB=0b11111111;
	uint8_t speed_val = 255;
	
	while(1)
	{
		pwminit();
		
		char data=uartGetByte();
		if (data == 'i')
		{
			if(speed_val<255)
			{
				speed_val = speed_val + 5;
			}	
			rprintf("%d\n",speed_val);
		}
		else if (data == 'j')
		{
			if(speed_val>0)
			{
				speed_val = speed_val - 5;
			}
			rprintf("%d\n",speed_val);
		}
		
		else if(data == 's')
		{
			setpwmoutput(speed_val);
			rprintf("reverse %d\n",speed_val);
			rprintf("%c\n\r",data);
			PORTB|= 0b00100000;
			_delay_ms(25);
  		    
		}
		else if(data == 'w')
		{
			setpwmoutput(speed_val);
			rprintf("forward  %d\n",speed_val);
			rprintf("%c\n\r",data);
			PORTB= 0b01000000;
			_delay_ms(20);		
		}
		
		else
		{
			PORTB=0b00000000;
		}
	}
}
	