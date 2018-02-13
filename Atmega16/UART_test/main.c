#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "SOR_Utils.h"

int main (void)
{
	uartInit();  // initialize UART
	uartSetBaudRate(115200);// set UART baud rate
	rprintfInit(uartSendByte);// initialize rprintf system

	while(1)
	{
		rprintf("\b");
	}
}
