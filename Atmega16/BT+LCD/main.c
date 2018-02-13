#define F_CPU 16000000UL
#define MrLCDsCrib PORTB
#define DataDir_MrLCDsCrib DDRB
#define MrLCDsControl PORTD
#define DataDir_MrLCDsControl DDRD
#define LightSwitch 5
#define ReadWrite 6
#define BiPolarMood 2

#include <avr/io.h>
#include <util/delay.h>
#include "global.h"
#include "SOR_Utils.h"




void Check_IF_MrLCD_isBusy(void);
void Peek_A_Boo(void);
void Send_A_Command(unsigned char command);
void Send_A_Character(unsigned char character);
void Send_A_String(char *StringOfCharacters);





int main (void)
{
	DDRA |= 0b11111111;
	PORTA |= 0b00000000;
	//TCCR1B = 1<< CS12;

	//char password;
	//int timercount = 0;
	//double time;
	uartInit();
	uartSetBaudRate(9600);
	rprintfInit(uartSendByte);
	char temp;
	
	

	DataDir_MrLCDsControl |= 1<<LightSwitch | 1<<ReadWrite | 1<<BiPolarMood;
	_delay_ms(15);

	Send_A_Command(0x01); //Clear Screen 0x01 = 00000001
	_delay_ms(2);
	Send_A_Command(0x38);
	_delay_us(50);
	Send_A_Command(0b00001101);
	_delay_us(50);
	
	
	Send_A_String("Enter Password");
	
	rprintf("Enter Password \n");
	
	while(1)
	{
	temp = uartGetByte();
		if(temp=='1')
		{
		while(1)
		{
		temp=uartGetByte();
		_delay_ms(500);
		if(temp=='2')
			{
				while(1)
				{
				temp=uartGetByte();
				_delay_ms(500);		
		if(temp=='a')
			{
			Send_A_Command(0x01);
			rprintf("Access Granted\n");
			Send_A_String("Access Granted");
	
			PORTA |= 0b11111111;
			_delay_ms(15000);
			PORTA |= 0b00000000;
			
			}
			else if(temp!='a')
			{
			Send_A_Command(0x01);
			_delay_ms(2);
			Send_A_String("Access Denied");
	
			rprintf("Access Denied \n");
			
			//asm volatile ("nop");
			//Send_A_Command(0x01);
			//Send_A_String("Access Denied");
			
	
	
	}
	}
	}
		}
		}
		}
		}
		//else if (temp!='1342')
		//{
			//Send_A_Command(0x01);
			//Send_A_String("Access Denied");
	
			//rprintf("\n Maa Chuda");
		//}
		
	//while (1)
	//{
	//temp =	uartGetByte();
	//rprintf("%c",temp);
	//rprintf("Received \n ");
	
	
	


void Check_IF_MrLCD_isBusy()
{
DataDir_MrLCDsCrib = 0;
MrLCDsControl |= 1<<ReadWrite;
MrLCDsControl &= ~1<<BiPolarMood;

while (MrLCDsCrib >= 0x80)
{
Peek_A_Boo();
}

DataDir_MrLCDsCrib = 0xFF; //0xFF means 0b11111111
}


void Peek_A_Boo()
{
MrLCDsControl |= 1<<LightSwitch;
asm volatile ("nop");
asm volatile ("nop");
MrLCDsControl &= ~1<<LightSwitch;
}



void Send_A_Command(unsigned char command)
{
Check_IF_MrLCD_isBusy();
MrLCDsCrib = command;
MrLCDsControl &= ~ ((1<<ReadWrite)|(1<<BiPolarMood));
Peek_A_Boo();
MrLCDsCrib = 0;
}

void Send_A_Character(unsigned char character)
{
Check_IF_MrLCD_isBusy();
MrLCDsCrib = character;
MrLCDsControl &= ~ (1<<ReadWrite);
MrLCDsControl |= 1<<BiPolarMood;
Peek_A_Boo();
MrLCDsCrib = 0;
}

void Send_A_String(char *StringOfCharacters)
{
while(*StringOfCharacters > 0)
{
Send_A_Character(*StringOfCharacters++);
_delay_us(50);
}
}