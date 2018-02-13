#define F_CPU 000000Ul
#include <avr/io.h>
#include “lcd.h”
#include <util/delay.h>
#include "SoR_Utils.h"
#include "global.h"


void print(uint16_t parameter)
{
	parameter%=10000;
	lcd_putc((parameter/1000)+48);
	parameter%=1000;
	lcd_putc((parameter/100)+48);
	parameter%=100;
	lcd_puts(“.”);
	lcd_putc((parameter/10)+48);
	lcd_putc((parameter%10)+48);
}
void main()
{

	a2dInit();
	a2dSetPrescaler(ADC_PRESCALE_DIV128);
	a2dSetReference(ADC_REFERENCE_AVCC);
	int adc;
	while(1)
	{
		lcd_init(LCD_DISP_ON);
		lcd_clrscr();
		lcd_gotoxy(0,0);
		lcd_puts(“Volts”);
		lcd_gotoxy(8,0);
		lcd_puts(“Amperes”);
		lcd_gotoxy(0,1);
		adc=a2dConvert8bit(0);    //Change to a2dConvert8bit
		print(adc*5);
		lcd_puts(“V”);
		InitADC();
		adc=a2dConvert8bit(0);
		lcd_gotoxy(8,1);
		print(adc*10);
		lcd_puts(“A”);
		_delay_ms(100);
	}
}