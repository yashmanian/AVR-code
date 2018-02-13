#include "All_libs.h"
#include "global.h"

void main()
{
	char data;
	uartInit();
	uartSetBaudRate(19200);
	rprintf("beginning LCD test\n");
	LCDInit(LS_BLINK|LS_ULINE);
	LCDClear();
	LCDWriteString("Beginning UART Test....");
	LCDWriteStringXY(0,1,"Press any key....");
	while(1)
	{
		data=uartGetByte();
		rprintf(data);
		LCDClear();
		LCDWriteString(data);
	}
}
		
	