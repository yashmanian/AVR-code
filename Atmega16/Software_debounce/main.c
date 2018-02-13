#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupts.h>
int main (void)
{
  DDRB |= 1 << PINB0;   //Data direction register output on pin B0 (LED1)
  PORTB ^= 1 << PINB0;
  DDRB &= ~(1 << PINB1);   //Data direction register input on pin B1 (button)
  PORTB |= 1 << PINB1;   //Initialise Pin B1 to HIGH(5v)
  DDRB |= 1 << PINB2;   //Data direction register output on pin B2 (LED2)
  
  int pressed = 0;
  int pressed_count = 0;
  int released_count = 0;
  while(1)
  {
   if(bit_is_clear(PINB,1))   //Checks if button is pressed
   {
     pressed_count++;
	 if(pressed_count > 500)
	 {
	 if(pressed == 0) 
	 {
	 PORTB ^= 1 << PINB0;
	 PORTB ^= 1 << PINB2;
	 pressed = 1;
	 pressed_count = 0;
	 }
	}
	//This block of code executes if button is not pressed
	else 
	{
	released_count++;
	if(released_count > 500)
	{
	 pressed = 0;
	 released_count = 0;
	}
	}
	}
  }
  }