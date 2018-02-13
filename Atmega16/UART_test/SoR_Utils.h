/****************************************************************************
*
*   Copyright (c) 2007 www.societyofrobots.com
*   (please link back if you use this code!)
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation.
*
*   Alternatively, this software may be distributed under the terms of BSD
*   license.
*
*	SoR Utilities v1, March 10th, 2007
*
****************************************************************************/

//AVR includes
#include <avr/io.h>		    // include I/O definitions (port names, pin names, etc)
#include <avr/interrupt.h>	// include interrupt support

//AVRlib includes
#include "global.h"		// include global settings
#include "buffer.h"		// include buffer function library
#include "uart.h"		// include uart function library
#include "rprintf.h"	// include printf function library
//#include "timer.h"		// include timer function library (timing, PWM, etc)
#include "a2d.h"		// include A/D converter function library

//define port functions; example: PORT_ON( PORTD, 6);
#define PORT_ON( port_letter, number )			port_letter |= (1<<number)
#define PORT_OFF( port_letter, number )			port_letter &= ~(1<<number)
#define PORT_ALL_ON( port_letter, number )		port_letter |= (number)
#define PORT_ALL_OFF( port_letter, number )		port_letter &= ~(number)
#define FLIP_PORT( port_letter, number )		port_letter ^= (1<<number)
#define PORT_IS_ON( port_letter, number )		( port_letter & (1<<number) )
#define PORT_IS_OFF( port_letter, number )		!( port_letter & (1<<number) )


//************CONFIGURE PORTS************
//configure ports for input or output - specific to ATmega8
void configure_ports(void)
	{
	DDRA = 0x00;  //configure all A ports for input
	PORTA = 0x00; //make sure pull-up resistors are turned off
	DDRD = 0xFF;  //configure all D ports for output
	DDRB = 0xFF;  //configure B ports 0, 1, 2, 6, 7 for output (google search '0b11000111 to hex')
	}
//***************************************

//************DELAY FUNCTIONS************
//wait for X amount of cycles (23 cycles is about .992 milliseconds)
//to calculate: 23/.992*(time in milliseconds) = number of cycles
void delay_cycles(unsigned long int cycles)
	{
	cycles=cycles*16;
	while(cycles > 0)
		cycles--;
	}
//***************************************

//*********SIMPLIFIED FUNCTIONS**********
//functions to make coding easier for a beginner
//but could cause port mixup confusion for intermediate users
void LED_on(void)
	{
	PORT_OFF(PORTD, 6);//turn LED on
	}
void LED_off(void)
	{
	PORT_ON(PORTD, 6);//turn LED off
	}
void servo_left(signed long int speed)
	{
	PORT_ON(PORTD, 2);
	delay_cycles(speed);
	PORT_OFF(PORTD, 2);//keep off
	delay_cycles(200);
	}
void servo_right(signed long int speed)
	{
	PORT_ON(PORTD, 3);
	delay_cycles(speed);		
	PORT_OFF(PORTD, 3);//keep off
	delay_cycles(200);
	}
//***************************************
