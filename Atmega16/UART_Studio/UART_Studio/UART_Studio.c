/*
 * UART_Studio.c
 *
 * Created: 11-09-2014 08:07:14
 *  Author: Yash Manian
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "SOR_Utils.h"
#include "global.h"

int main(void)
{
	uartInit();
	uartSetBaudRate(9600);
	rprintfInit(uartSendByte);
	char temp;
	
    while(1)
    {
       temp=uartGetByte(); //TODO:: Please write your application code
	   rprintf("%c",temp); 
    }
}