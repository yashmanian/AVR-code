/*
 * GccApplication1.c
 *
 * Created: 13-09-2014 08:33:43
 *  Author: Yash Manian
 */ 

#include "SoR_Utils.h"
#include "global.h"

int main(void)
{
	uartInit();					// initialize UART (serial port)
	uartSetBaudRate(9600);		// set UART speed to 9600 baud
	rprintfInit(uartSendByte);
	char temp=0;
    while(1)
    {
        temp=uartGetByte();//TODO:: Please write your application code
		rprintf("received : %c",temp); 
    }
}