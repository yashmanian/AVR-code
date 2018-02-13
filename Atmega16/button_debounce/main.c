#include <avr/io.h>
int main (void)
{
  DDRB |= 1 << PINB0;   //Data direction register output on pin B0 (LED1)
  PORTB ^= 1 << PINB0;
  DDRB &= ~(1 << PINB1);   //Data direction register input on pin B1 (button)
  PORTB |= 1 << PINB1;   //Initialise Pin B1 to HIGH(5v)
  DDRB |= 1 << PINB2;   //Data direction register output on pin B2 (LED2)
  int pressed = 0;
  while(1)
  {
   if(bit_is_clear(PINB,1))   //Checks if button is pressed
   {
    //Make sure button is released first
	if(pressed == 0)
	{
	PORTB ^= 1 << PINB0;
	PORTB ^= 1 << PINB2;
	pressed = 1;
	}
	//This block of code executes if button is not pressed
	else 
	{
	pressed = 0;
	}
	}
  }
  }