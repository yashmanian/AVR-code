/*
 * UART_Test6.c
 *
 * Created: 12-09-2014 08:19:51
 *  Author: Yash Manian
 */ 


#include <avr/io.h>
#include "global.h"
#include <util/delay.h>
#include "SOR_Utils.h"

int main (void)
{
	uartInit();			//initialise UART
	uartSetBaudRate(9600);		//set UART baudrate
	rprintfInit(uartSendByte);		//rprintf function to send byte through uart
	int sensor_1 = 0;		//sensor 1 and 2 as integer values
	configure_ports();		//set i/p and o/p ports
	a2dInit();		//Initialise ADC
	a2dSetPrescaler(ADC_PRESCALE_DIV128);		//Set prescaler of 128 for ADC conversion
	a2dSetReference(ADC_REFERENCE_AVCC);		//Set reference voltage for ADC as AVCC
	
	
	
	while(1)
	{
		//Store sensor data
		sensor_1 = a2dConvert10bit(1);
		//Print sensor data through UART
		rprintf("%d\n\r",sensor_1);
	}
}