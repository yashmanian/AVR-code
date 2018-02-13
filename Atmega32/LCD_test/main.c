#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

void main()
{
   unsigned char i;

   //Initialize LCD module
   LCDInit(LS_BLINK|LS_ULINE);

   //Clear the screen
   LCDClear();

   //Simple string printing
   LCDWriteString("Congrats ");

   //A string on line 2
   LCDWriteStringXY(0,1,"Loading ");

   //Print some numbers
   for (i=0;i<99;i+=1)
   {
      LCDWriteIntXY(9,1,i,3);
      LCDWriteStringXY(12,1,"%");
      _delay_loop_2(0);
      _delay_loop_2(0);
      _delay_loop_2(0);
      _delay_loop_2(0);

   }

   //Clear the screen
   LCDClear();

   //Some more text

   LCDWriteString("Hello world");
   LCDWriteStringXY(0,1,"Yashmaniac");    // <--- Write ur NAME HERE !!!!!!!!!!!

   //Wait
   for(i=0;i<100;i++) _delay_loop_2(0);

   //Some More ......
   LCDClear();
   LCDWriteString("    Planet");
   LCDWriteStringXY(0,1,"  Manian");

}