#include <avr/io.h>
#include <util/delay.h>
void main (void)
{
  DDRB |= 1 << PINB0;   //Data direction register output on pin B0
  DDRB &= ~(1 << PINB1);   //Data direction register input on pin B1
  PORTB |= 1 << PINB1;   //Initialise Pin B1 to HIGH(5v)
  while(1)
  {
   PORTB ^= 1 << PINB0;
   if(bit_is_clear(PINB,1))
   {
    _delay_ms(1000);   //Pressing button makes blink faster
   }
   else
   {
   _delay_ms(100);   //release makes blink slower
   }
  }
}  