/*
 * UART_Lib2.c
 *
 * Created: 14-09-2014 09:32:20
 *  Author: Yash Manian
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include "uart.h"
#include "rprintf.h"
#include "buffer.h"
#include "global.h"

int main(void)
{
	uartInit();
	uartSetBaudRate(9600);
	rprintfInit(uartSendByte);
	char data;
    while(1)
    {
		data = uartGetByte();
        rprintf("received data = %c",data)//TODO:: Please write your application code 
    }
}