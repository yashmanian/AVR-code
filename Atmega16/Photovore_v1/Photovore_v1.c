

//SoR Include
#include "SoR_Utils.h" //includes all the technical stuff


int main(void)
	{
	//declare variables here
	//int i=250;//a 'whatever' variable
	int sensor_left=0;//left photoresistor
	int sensor_right=0;//right photoresistor
	int threshold=8;//the larger this number, the more likely your robot will drive straight
	uartInit();  // initialize UART
	uartSetBaudRate(38400);// set UART baud rate
	rprintfInit(uartSendByte);// initialize rprintf system
	

	/****************INITIALIZATIONS*******************/
	//other stuff Im experimenting with for SoR
	//uartInit();  // initialize the UART (serial port)
	//uartSetBaudRate(9600);// set the baud rate of the UART for our debug/reporting output
	//rprintfInit(uartSendByte);// initialize rprintf system

	//timerInit(); // initialize the timer system
	
	configure_ports(); // configure which ports are analog, digital, etc.
	a2dInit(); // initialize analog to digital converter (ADC)
	a2dSetPrescaler(ADC_PRESCALE_DIV32); // configure ADC scaling
	a2dSetReference(ADC_REFERENCE_AVCC); // configure ADC reference voltage

	//rprintf("Initialization Complete\r\n");
	/**************************************************/

	
	/*********ADD YOUR CODE BELOW THIS LINE **********/
	LED_off();//turn LED on


	while(1)
		{
		//store sensor data
		sensor_left=a2dConvert8bit(5);
		sensor_right=a2dConvert8bit(4);


		//detects more light on left side of robot
		if(sensor_left > sensor_right && (sensor_left - sensor_right) > threshold)
			{//go left
			servo_left(44);
			servo_right(44);
			}

		//detects more light on right side of robot
		else if(sensor_right > sensor_left && (sensor_right - sensor_left) > threshold)
			{//go right
			servo_left(25);
			servo_right(25);
			}

		//light is about equal on both sides
		else
			{//go straight
			servo_left(25);
			servo_right(44);
			}


		/* Servo Test Code
		i=250;
		while(i>0)
			{
			servo_left(40);
			i--;
			}

		i=250;
		while(i>0)
			{
			servo_left(24);
			i--;
			}
		*/

		//rprintf("Initialization Complete\r\n");
		
		//output message to serial (use hyperterminal)
		rprintf("Hello, World! My Analog: %d\r\n", sensor_left);//print("Hello, World! Read My Analog: %u\r\n", sensor_0);

		delay_cycles(500);//a small delay to prevent crazy oscillations
		}
	/*********ADD YOUR CODE ABOVE THIS LINE **********/

	return 0;
	}


/*********************COMMAND LIST*************************

delay_cycles(cycles);
Delays - you can make your robot wait for a certain amount of time with this function.
Put the number of computational cycles to delay in the ().
23 cycles is about .992 milliseconds
to calculate: 23/.992*(time in milliseconds to delay) = cycles
Check servo datasheet where it says: 'Direction: Clockwise/Pulse Traveling 1500 to 1900usec'


servo_left(speed); and servo_right(speed);
Commands your servos to rotate at a certain speed.
Vary speed (which represents a delay in cycles) from 20 to 50.
Left is for port D0 and right is for port D1.


LED_on(); and LED_off();
Turns on and off your LED. The LED is on port D4.
By bringing port D4 low, you are turning on the LED.


variable=a2dConvert8bit(pin);
Reads analog pin. For example, set 'pin' to 5 to read PC5.
'variable' will store the value.

***********************************************************/
