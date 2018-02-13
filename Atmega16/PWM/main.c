
#include <avr/io.h>
#include <util/delay.h>
#include "SoR_Utils.h"
#include "global.h"

void InitPWM()
{
		TCCR1A|=(1<<WGM10)|(1<<COM1A1);//|(1<<COM1A0);
		TCCR1B|=(1<<WGM12)|(1<<CS10);
      
		DDRD|=(1<<PIND5); // set output for PWM timer 1
}


void SetPWMOutput(uint8_t duty)
{
   OCR1A = duty;
}

/******************************************************************** 

Simple Wait Loop

*********************************************************************/

void Wait()
{
 _delay_loop_2(10000);
}

void main()
{
	uartInit();
	uartSetBaudRate(9600);
	rprintfInit(uartSendByte);
   uint8_t brightness=0;

   //Initialize PWM Channel 0
   InitPWM();
   a2dInit();		//Initialise ADC
	a2dSetPrescaler(ADC_PRESCALE_DIV128);		//Set prescaler of 128 for ADC conversion
	a2dSetReference(ADC_REFERENCE_AVCC);		//Set reference voltage for ADC as AVCC

   //Do this forever

   while(1)
   {
      int sensorData= a2dConvert8bit(1);
	  SetPWMOutput(sensorData);
	  
	  rprintf("%d\n",sensorData);
	}
}