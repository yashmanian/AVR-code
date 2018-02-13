#define F_CPU 8000000UL
#include <avr/io.h>
#include "global.h"
#include <util/delay.h>
#include "SOR_Utils.h"

int main()
{
	uartInit();
	uartSetBaudRate(2400);
	rprintfInit(uartSendByte);
	
	while(1)
	{
		char data=uartGetByte();
		rprintf("baka\n");
		
	
	}
}
	