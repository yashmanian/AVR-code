#define F_CPU 16000000UL
#include <avr/io.h>
#include “lcd.h”
#include <util/delay.h>
void InitADC()
{
ADMUX=(1<<REFS0);                         // For Aref=AVcc;
ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Prescalar div factor =128
}
uint16_t ReadADC(uint8_t ch)
{
ch=ch&0b00000111;
ADMUX|=ch;
ADCSRA|=(1<<ADSC);
while(!(ADCSRA & (1<<ADIF)));
ADCSRA|=(1<<ADIF);
return(ADC);
}
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
InitADC();
adc=ReadADC(0);
print(adc*5);
lcd_puts(“V”);
InitADC();
adc=ReadADC(1);
lcd_gotoxy(8,1);
print(adc*10);
lcd_puts(“A”);
_delay_ms(100);
}
}