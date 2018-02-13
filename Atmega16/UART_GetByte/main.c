#include "SOR_Utils.h"
#include "global.h"

char temp;
int main (void)
{
		uartInit();
		uartSetBaudRate(9600);
		rprintfInit(uartSendByte);
		rprintf("UART Initialised...");
		
	while(1)
	{
		temp = uartGetByte();
		
	    rprintf("%c\n",temp);
		
	
	}
}

