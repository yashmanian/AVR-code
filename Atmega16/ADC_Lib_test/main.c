#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "SOR_Utils.h"

void InitPWM()
{
   /*
   TCCR0 - Timer Counter Control Register (TIMER0)
   -----------------------------------------------
   BITS DESCRIPTION
   

   NO:   NAME   DESCRIPTION
   --------------------------
   BIT 7 : FOC0   Force Output Compare [Not used in this example]
   BIT 6 : WGM00  Wave form generartion mode [SET to 1]
   BIT 5 : COM01  Compare Output Mode        [SET to 1]
   BIT 4 : COM00  Compare Output Mode        [SET to 0]

   BIT 3 : WGM01  Wave form generation mode [SET to 1]
   BIT 2 : CS02   Clock Select               [SET to 0]
   BIT 1 : CS01   Clock Select               [SET to 0]
   BIT 0 : CS00   Clock Select               [SET to 1]

   The above settings are for
   --------------------------

   Timer Clock = CPU Clock (No Prescalling)
   Mode        = Fast PWM
   PWM Output  = Non Inverted

   */


   TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS00);

   //Set OC0 PIN as output. It is  PB3 on ATmega16 ATmega32

   DDRB|=(1<<PB3);
}

void SetPWMOutput(uint8_t duty)
{
   OCR0=duty;
}


int main()
{
	a2dInit();
	a2dSetPrescaler(ADC_PRESCALE_DIV32);
	a2dSetReference(ADC_REFERENCE_AVCC);
	uint8_t sensor_val1 = 0;
	DDRD|=1<<PIND6;
	uint8_t brightness=0;

	//Initialize PWM Channel 0
	InitPWM();

	while(1)
	{
		brightness = a2dConvert8bit(0);
		SetPWMOutput(brightness);
		
	}
}