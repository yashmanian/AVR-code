/*
 * UART_Lib.c
 *
 * Created: 14-09-2014 09:22:00
 *  Author: Yash Manian
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "SoR_Utils.h"
#include "global.h"
int main(void)
{
	uartInit();
	uartSetBaudRate(9600);
	rprintfInit(uartSendByte);
	char data;
    while(1)
    {
        data= uartGetbyte();//TODO:: Please write your application code
		rprintf("data = %c\n",data); 
    }
}