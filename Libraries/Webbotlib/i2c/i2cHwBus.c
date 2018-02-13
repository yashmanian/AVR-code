/*
 * $Id: i2cHwBus.c,v 1.3 2010/10/01 19:55:03 clivewebster Exp $
 * Revision History
 * ================
 * $Log: i2cHwBus.c,v $
 * Revision 1.3  2010/10/01 19:55:03  clivewebster
 * Setting the bit rate only applies to a hardware master
 *
 * Revision 1.2  2010/10/01 15:28:11  clivewebster
 * Refactored - merge master and slave into i2cBus.h
 *
 * Revision 1.1  2010/09/30 16:44:01  clivewebster
 * Refactored to replace i2c_master.h
 *
 * ================
 *
 * Copyright (C) 2010 Clive Webster (webbot@webbot.org.uk)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *        File: i2cHwBus.c
 *  Created on: 24 Sep 2010
 *      Author: Clive Webster
 *
 *  Place a thin wrapper around the i2c_master calls so that they can be used
 *  from an abstract i2c bus
 */
#include "../i2cBus.h"

#ifdef I2C_HARDWARE

// TWS4:3 set - ACK has been received back
// Acknowledgement in Write mode
#define TW_MT_SLA_ACK				0x18

// TWS6 set
// Acknowledgement in Read mode
#define TW_MR_SLA_ACK				0x40

// The sent data has been acknowledged
#define TW_MT_DATA_ACK				0x28

// Include the device definitions
#include <avr/io.h>

static void speed(const I2C_ABSTRACT_BUS* bus,uint16_t bitrateKHz){
	// TWBR x 4pow(TWPS) = ((cpudiv1000/ bitrateKHz) - 16) /2
	uint16_t bitrate_div;

	// calculate bitrate division
	bitrate_div = cpu_speed_div_1000 / bitrateKHz;

	if(bitrate_div >= 16){
		bitrate_div = (bitrate_div-16)/2;
	}else{
		bitrate_div=0;
	}

	// Turn prescaler off by default
	#ifdef TWPS0
		cbi(TWSR, TWPS0);
	#endif
	#ifdef TWPS1
		cbi(TWSR, TWPS1);
	#endif

	// Check if need to use the prescaler
	#ifdef TWPS0
	if(bitrate_div > 255){
		sbi(TWSR, TWPS0);
		bitrate_div /= 4;
	}
	#endif
	#ifdef TWPS1
	if(bitrate_div > 255){
		sbi(TWSR, TWPS1);
		bitrate_div /= 16;
	}
	#endif

	// Cannot prescale it on some devices - so just go as slow as possible
	if(bitrate_div > 255){
		bitrate_div = 255;
	}

	outb(TWBR, (uint8_t)(bitrate_div) );
}


// Initialiase I2C hardware
void __attribute__ ((constructor)) _private_i2cMasterInit(void){

	// Make the 2 wires into inputs
	cbi(SCL_DDR,SCL_PIN);
	cbi(SDA_DDR,SDA_PIN);
	// and turn on the pullups
	sbi(SCL_PORT,SCL_PIN);
	sbi(SDA_PORT,SDA_PIN);


	// enable TWI (two-wire interface)
	sbi(TWCR, TWEN);

	// set state
	i2cState = I2C_IDLE;

	// enable TWI interrupt and slave address ACK
//	sbi(TWCR, TWIE);
//	sbi(TWCR, TWEA);

	// enable interrupts
//	sei();
}

static void init(I2C_ABSTRACT_BUS* bus){
	// set i2c bit rate to 100KHz
	speed(bus,100);
}

static inline void i2cWaitForComplete(void){
	// wait for i2c interface to complete operation
	while( !(inb(TWCR) & BV(TWINT)) );
}

static uint8_t getByte(const I2C_ABSTRACT_BUS* bus, boolean isLastByte){
	// begin receive over i2c
	if( isLastByte ){
		// ackFlag = FALSE: NACK the received data
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
	}else{
		// ackFlag = TRUE: ACK the recevied data
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
	}
	i2cWaitForComplete();
	return inb(TWDR);
}


static boolean putByte(const I2C_ABSTRACT_BUS* bus, uint8_t data){
	// save data to the TWDR
	outb(TWDR, data);
	// begin send
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));

	i2cWaitForComplete();

	uint8_t got = inb(TWSR) & TWSR_STATUS_MASK;	// get the status

    return( (got == TW_MT_DATA_ACK) ? TRUE : FALSE );

}

static boolean start(const I2C_ABSTRACT_BUS* i2c, uint8_t addr, boolean writeMode){
	uint8_t expect;
	if(writeMode){
		i2cState = I2C_MASTER_TX;
		expect = TW_MT_SLA_ACK;
		addr &= 0xfe;
	}else{
		i2cState = I2C_MASTER_RX;
		expect = TW_MR_SLA_ACK;
		addr |= 1;
	}

	// send start condition
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWSTA));
	i2cWaitForComplete();

	// Send the device addr and direction
	outb(TWDR, addr);
	// begin send
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
	i2cWaitForComplete();

	uint8_t got = inb(TWSR) & TWSR_STATUS_MASK;	// get the status
    return( (got == expect) ? TRUE : FALSE );

}

// transmit stop condition
static void stop(const I2C_ABSTRACT_BUS* bus){
	// leave with TWEA on for slave receiving
	outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA)|BV(TWSTO));
	i2cState = I2C_IDLE;
}


// Expose this implementation to the linker
const I2C_CLASS PROGMEM c_hw_i2c = MAKE_I2C_CLASS(&init, &start,&stop, &getByte, &putByte, &speed);

#elif defined(I2C_OVER_USI)

#include "../core.h"

// See ATmel AVR310

/*
#define SYS_CLK   4000.0  // [kHz] = F_CPU / 1000
#ifdef TWI_FAST_MODE               // TWI FAST mode timing limits. SCL = 100-400kHz
  #define T2_TWI    ((SYS_CLK *13) /10000) +1 // >1,3us
  #define T4_TWI    ((SYS_CLK * 6) /10000) +1 // >0,6us
#else                              // TWI STANDARD mode timing limits. SCL <= 100kHz
  #define T2_TWI    ((SYS_CLK *47) /10000) +1 // >4,7us
  #define T4_TWI    ((SYS_CLK *40) /10000) +1 // >4,0us
#endif
*/

#define tempUSISR_8bit  (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      /* Prepare register value to: Clear flags, and */ \
                                (0x0<<USICNT0)                              /* set USI to shift 8 bits i.e. count 16 clock edges. */
#define tempUSISR_1bit  (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      /* Prepare register value to: Clear flags, and */ \
                                (0xE<<USICNT0)                              /* set USI to shift 1 bit i.e. count 2 clock edges.*/
#define TWI_NACK_BIT  0       // Bit position for (N)ACK bit.


static uint8_t t2;
static uint8_t t4;
static uint8_t tstart;

/*---------------------------------------------------------------
 Core function for shifting data in and out from the USI.
 Data to be sent has to be placed into the USIDR prior to calling
 this function. Data read, will be return'ed from the function.
---------------------------------------------------------------*/
static uint8_t USI_TWI_Master_Transfer( uint8_t temp ) {
  USISR = temp;                                     // Set USISR according to temp.
                                                    // Prepare clocking.
  temp  =  (0<<USISIE)|(0<<USIOIE)|                 // Interrupts disabled
           (1<<USIWM1)|(0<<USIWM0)|                 // Set USI in Two-wire mode.
           (1<<USICS1)|(0<<USICS0)|(1<<USICLK)|     // Software clock strobe as source.
           (1<<USITC);                              // Toggle Clock Port.
  do
  {
	_delay_loop_1( t2 );
    USICR = temp;                          // Generate positive SCL edge.
    while( bit_is_clear(PIN_USI,PIN_USI_SCL) );// Wait for SCL to go high.
    _delay_loop_1( t4 );
    USICR = temp;                          // Generate negative SCL edge.
  }while( !(USISR & (1<<USIOIF)) );        // Check for transfer complete.

  _delay_loop_1( t2);
  temp  = USIDR;                           // Read out data.
  USIDR = 0xFF;                            // Release SDA.
  sbi(DDR_USI,PIN_USI_SDA);                // Enable SDA as output.

  return temp;                             // Return the data from the USIDR
}


// Initialiase I2C hardware on startup
void __attribute__ ((constructor)) _private_i2cMasterInit(void){
	// set state
	i2cState = I2C_IDLE;

	cbi(DDR_USI,PIN_USI_SDA);           // Enable SDA as input.
	sbi(PORT_USI,PIN_USI_SDA);           // Enable pullup on SDA, to set high as released state.
	cbi(DDR_USI,PIN_USI_SCL);           // Enable SCL as input.
	sbi(PORT_USI,PIN_USI_SCL);           // Enable pullup on SCL, to set high as released state.

	sbi(DDR_USI,PIN_USI_SCL);           // Enable SCL as output.
	sbi(DDR_USI,PIN_USI_SDA);           // Enable SDA as output.

	USIDR    =  0xFF;                   // Preload data register with "released level" data.
	USICR    =  (0<<USISIE)|(0<<USIOIE)|                            // Disable Interrupts.
			  (1<<USIWM1)|(0<<USIWM0)|                            // Set USI in Two-wire mode.
			  (1<<USICS1)|(0<<USICS0)|(1<<USICLK)|                // Software strobe as counter clock source
			  (0<<USITC);
	USISR   =   (1<<USISIF)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Clear flags,
			  (0x0<<USICNT0);                                     // and reset counter.

}

static uint8_t scale(uint8_t delay){
	uint16_t kHz = cpu_speed_div_1000;
	uint32_t top = kHz;
	uint16_t div = 30000U;	// 3 cycles per _delay_loop

	top *= delay;
	top /= div;
	uint8_t rtn = top & 0xff;
	return ++rtn;
}

static void speed(const I2C_ABSTRACT_BUS* bus,uint16_t bitrateKHz){
	uint8_t t2mul;
	uint8_t t4mul;
	if (bitrateKHz > 100){
		// fast mode
		t2mul = 13; // >1.3us
		t4mul =  6; // >0.6us
	} else {
		// slow mode
		t2mul = 47; // >4.7us
		t4mul = 40; // >4.0us
	}
	t2 = scale(t2mul);
	t4 = scale(t4mul);
	if (bitrateKHz > 100){
		tstart = t4;
	} else {
		tstart = t2;
	}
}

static void init(I2C_ABSTRACT_BUS* bus){
	// set i2c bit rate to 100KHz
	speed(bus,100);
}

static boolean putByte(const I2C_ABSTRACT_BUS* bus, uint8_t data){
	/* Write a byte */
	cbi(PORT_USI,PIN_USI_SCL);                	// Pull SCL LOW.
	USIDR     = data;                        	// Setup data.
	USI_TWI_Master_Transfer( tempUSISR_8bit );  // Send 8 bits on bus.

	/* Clock and verify (N)ACK from slave */
	cbi(DDR_USI,PIN_USI_SDA);                	// Enable SDA as input.
	if( USI_TWI_Master_Transfer( tempUSISR_1bit ) & (1<<TWI_NACK_BIT) ) {
		return false;
	}

	return true;
}
// Send a start condition along with the device addr
static boolean start(const I2C_ABSTRACT_BUS* bus, uint8_t addr, boolean writeMode){
	if(writeMode){
		addr &= 0xfe;
		i2cState = I2C_MASTER_TX;
	}else{
		addr |= 1;
		i2cState = I2C_MASTER_RX;
	}

	/* Release SCL to ensure that (repeated) Start can be performed */
	sbi(PORT_USI,PIN_USI_SCL);                     		// Release SCL.
	while( bit_is_clear(PORT_USI,PIN_USI_SCL) );        // Verify that SCL becomes high.
	_delay_loop_1(tstart);

	/* Generate Start Condition */
	cbi(PORT_USI,PIN_USI_SDA);                    		// Force SDA LOW.
	_delay_loop_1( t4 );
	cbi(PORT_USI,PIN_USI_SCL);                    		// Pull SCL LOW.
	sbi(PORT_USI,PIN_USI_SDA);                     		// Release SDA.

    /* Write address byte */
	return putByte(bus,addr);
}


// transmit stop condition
static void stop(const I2C_ABSTRACT_BUS* bus){
	i2cState = I2C_IDLE;
	cbi(PORT_USI,PIN_USI_SDA);           // Pull SDA low.
	sbi(PORT_USI,PIN_USI_SCL);            // Release SCL.
	while( bit_is_clear(PIN_USI,PIN_USI_SCL));  // Wait for SCL to go high.
	_delay_loop_1( t4 );
	sbi(PORT_USI,PIN_USI_SDA);            // Release SDA.
	_delay_loop_1( t2 );
}

static uint8_t getByte(const I2C_ABSTRACT_BUS* bus, boolean isLastByte){
	/* Read a data byte */
	cbi(DDR_USI,PIN_USI_SDA);               	// Enable SDA as input.
	uint8_t rtn  = USI_TWI_Master_Transfer( tempUSISR_8bit );

	/* Prepare to generate ACK (or NACK in case of End Of Transmission) */
	USIDR = (isLastByte) ? 0xFF : 0x00;
	USI_TWI_Master_Transfer( tempUSISR_1bit );   // Generate ACK/NACK.

	return rtn;
}



// Expose this implementation to the linker
const I2C_CLASS PROGMEM c_hw_i2c = MAKE_I2C_CLASS(&init, &start,&stop, &getByte, &putByte, &speed);

#endif
