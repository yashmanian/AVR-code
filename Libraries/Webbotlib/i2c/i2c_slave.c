/*
 * $Id: i2c_slave.c,v 1.3 2010/10/01 19:55:03 clivewebster Exp $
 * Revision History
 * ================
 * $Log: i2c_slave.c,v $
 * Revision 1.3  2010/10/01 19:55:03  clivewebster
 * Setting the bit rate only applies to a hardware master
 *
 * Revision 1.2  2010/10/01 13:17:04  clivewebster
 * *** empty log message ***
 *
 * Revision 1.1  2010/09/30 16:44:49  clivewebster
 * Added
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
 *        File: i2c_slave.c
 *  Created on: 26 Aug 2010
 *      Author: Clive Webster
 */

// Include the device definitions
#include <avr/io.h>
#include "../i2cBus.h"

#if defined( I2C_HARDWARE ) || defined(I2C_OVER_USI)
static cBuffer* rxBuffer;
static cBuffer* txBuffer;
static void (*i2cSlaveReceive)(cBuffer* data);
static void (*i2cSlaveTransmit)(cBuffer* data);

void i2cSlaveSetReceiveHandler(void (*i2cSlaveRx_func)(cBuffer* data)){
	CRITICAL_SECTION{
		i2cSlaveReceive = i2cSlaveRx_func;
	}
}
void i2cSlaveSetTransmitHandler(void (*i2cSlaveTx_func)(cBuffer* data)){
	CRITICAL_SECTION{
		i2cSlaveTransmit = i2cSlaveTx_func;
	}
}
#endif


#if defined( I2C_HARDWARE )
// ----- Add hardware I2C slave support --------------------

void i2cSlaveInit(uint8_t deviceAddr, boolean generalCall, cBuffer* rx, cBuffer* tx){
	// Make the 2 wires into inputs
	cbi(SCL_DDR,SCL_PIN);
	cbi(SDA_DDR,SDA_PIN);

	// Save the buffer addresses
	rxBuffer = rx;
	txBuffer = tx;

	// Only the master has pullups
	//sbi(SCL_PORT,SCL_PIN);
	//sbi(SDA_PORT,SDA_PIN);

	// set i2c bit rate to 100KHz
	// not for slaves as their bit rate is set by the master
//	i2cMasterSetBitrate(100);

	// enable TWI (two-wire interface)
	sbi(TWCR, TWEN);

	// set state
	i2cState = I2C_IDLE;

	// enable TWI interrupt and slave address ACK
	sbi(TWCR, TWIE);
	sbi(TWCR, TWEA);

	// Set the device address
	outb(TWAR, ((deviceAddr&0xFE) | (generalCall?1:0)) );

	// enable interrupts
	sei();

}

static inline void i2cReceiveByte(boolean ackFlag){
	// begin receive over i2c
	if( ackFlag ){
		// ackFlag = TRUE: ACK the received data
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
	}else{
		// ackFlag = FALSE: NACK the received data
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
	}
}


// I2C (TWI) interrupt service routine
ISR(TWI_vect)
{
	// read status bits
	uint8_t status = inb(TWSR) & TWSR_STATUS_MASK;
	uint8_t byte;

	switch(status)
	{
	// Slave Receiver status codes
	case 0x60:					// 0x60: own SLA+W has been received, ACK has been returned
	case 0x68:		// 0x68: own SLA+W has been received, ACK has been returned
	case 0x70:				// 0x70:     GCA+W has been received, ACK has been returned
	case 0x78:		// 0x78:     GCA+W has been received, ACK has been returned
		// we are being addressed as slave for writing (data will be received from master)
		// set state
		i2cState = I2C_SLAVE_RX;
		// prepare buffer
		bufferFlush(rxBuffer);
		// receive data byte and return ACK
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
		break;

	case 0x80:			// 0x80: data byte has been received, ACK has been returned
	case 0x90:			// 0x90: data byte has been received, ACK has been returned
		byte = inb(TWDR);
		if(rxBuffer){
			boolean ok = bufferPut(rxBuffer, byte);
			i2cReceiveByte(ok);
		}else{
			i2cReceiveByte(FALSE);
		}
		break;

	case 0x88:			// 0x88: data byte has been received, NACK has been returned
	case 0x98:			// 0x98: data byte has been received, NACK has been returned
		// receive data byte and return NACK
		i2cReceiveByte(FALSE);
		break;

	case 0xa0:					// 0xA0: STOP or REPEATED START has been received while addressed as slave
		// switch to SR mode with SLA ACK
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
		// i2c receive is complete, call i2cSlaveReceive
		if(i2cSlaveReceive && rxBuffer){
			i2cSlaveReceive(rxBuffer);
		}
		// set state
		i2cState = I2C_IDLE;
		break;

	// Slave Transmitter
	case 0xa8:					// 0xA8: own SLA+R has been received, ACK has been returned
	case 0xb0:					// 0xB0:     GCA+R has been received, ACK has been returned
		// we are being addressed as slave for reading (data must be transmitted back to master)
		// set state
		i2cState = I2C_SLAVE_TX;

		// request data from application
		if(i2cSlaveTransmit && txBuffer){
			// prepare buffer
			bufferFlush(txBuffer);
			i2cSlaveTransmit(txBuffer);
		}
		// fall-through to transmit first data byte
	case 0xb8:				// 0xB8: data byte has been transmitted, ACK has been received
		// transmit data byte
		bufferGet(txBuffer, &byte);
		outb(TWDR, byte);
		if(bufferBytesUsed(txBuffer)!=0){
			// expect ACK to data byte
			outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
		}else{
			// expect NACK to data byte
			outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT));
		}
		break;

	case 0xc0:				// 0xC0: data byte has been transmitted, NACK has been received
	case 0xc8:				// 0xC8:
		// all done
		// switch to open slave
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWEA));
		// set state
		i2cState = I2C_IDLE;
		break;

	// Miscellaneous settings
	case 0xf8:					// 0xF8: No relevant state information
		// do nothing
		break;

	case 0:						// 0x00: Bus error due to illegal start or stop condition
		// reset internal hardware and release bus
		outb(TWCR, (inb(TWCR)&TWCR_CMD_MASK)|BV(TWINT)|BV(TWSTO)|BV(TWEA));
		// set state
		i2cState = I2C_IDLE;
		break;
	}
}
#elif defined (I2C_OVER_USI)
// --------------- Add I2C slave hardware support for a Universal Serial Interface. See AVR312 --------
#define USI_SLAVE_CHECK_ADDRESS                (0x00)
#define USI_SLAVE_SEND_DATA                    (0x01)
#define USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA (0x02)
#define USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA   (0x03)
#define USI_SLAVE_REQUEST_DATA                 (0x04)
#define USI_SLAVE_GET_DATA_AND_SEND_ACK        (0x05)


//Functions implemented as macros
#define SET_USI_TO_SEND_ACK()                                                                                 \
{                                                                                                             \
        USIDR    =  0;                                              /* Prepare ACK                         */ \
        DDR_USI |=  (1<<PORT_USI_SDA);                              /* Set SDA as output                   */ \
        USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|  /* Clear all flags, except Start Cond  */ \
                    (0x0E<<USICNT0);                                /* set USI counter to shift 1 bit. */ \
}

#define SET_USI_TO_READ_ACK()                                                                                 \
{                                                                                                             \
        DDR_USI &=  ~(1<<PORT_USI_SDA);                             /* Set SDA as intput */                   \
        USIDR    =  0;                                              /* Prepare ACK        */                  \
        USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|  /* Clear all flags, except Start Cond  */ \
                    (0x0E<<USICNT0);                                /* set USI counter to shift 1 bit. */ \
}


#define SET_USI_TO_SEND_DATA()                                                                               \
{                                                                                                            \
    DDR_USI |=  (1<<PORT_USI_SDA);                                  /* Set SDA as output                  */ \
    USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      /* Clear all flags, except Start Cond */ \
                (0x0<<USICNT0);                                     /* set USI to shift out 8 bits        */ \
}

#define SET_USI_TO_READ_DATA()                                                                               \
{                                                                                                            \
    DDR_USI &= ~(1<<PORT_USI_SDA);                                  /* Set SDA as input                   */ \
    USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      /* Clear all flags, except Start Cond */ \
                (0x0<<USICNT0);                                     /* set USI to shift out 8 bits        */ \
}

#define SET_USI_TO_TWI_START_CONDITION_MODE()                                                                                     \
{	\
  i2cState = I2C_IDLE;\
  USICR    =  (1<<USISIE)|(0<<USIOIE)|                        /* Enable Start Condition Interrupt. Disable Overflow Interrupt.*/  \
              (1<<USIWM1)|(0<<USIWM0)|                        /* Set USI in Two-wire mode. No USI Counter overflow hold.      */  \
              (1<<USICS1)|(0<<USICS0)|(0<<USICLK)|            /* Shift Register Clock Source = External, positive edge        */  \
              (0<<USITC);                                                                                                         \
  USISR    =  (0<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|  /* Clear all flags, except Start Cond                            */ \
              (0x0<<USICNT0);                                                                                                     \
}


// The address of the slave device: lsb is always 0
static unsigned char TWI_slaveAddress;
static volatile unsigned char USI_TWI_Overflow_State;
static boolean allowGenCall;

void i2cSlaveInit(uint8_t deviceAddr, boolean generalCall, cBuffer* rx, cBuffer* tx){
	TWI_slaveAddress = deviceAddr>>1;
	allowGenCall = generalCall;

	// Save the buffer addresses
	rxBuffer = rx;
	txBuffer = tx;

	// set state
	i2cState = I2C_IDLE;

	// Turn on I2C
	PORT_USI |=  (1<<PORT_USI_SCL); 				// Set SCL high
	PORT_USI |=  (1<<PORT_USI_SDA);     			// Set SDA high
	DDR_USI  |=  (1<<PORT_USI_SCL);     			// Set SCL as output
	DDR_USI  &= ~(1<<PORT_USI_SDA);     			// Set SDA as input
	USICR  =  (1<<USISIE)|(0<<USIOIE)|  			// Enable Start Condition Interrupt. Disable Overflow Interrupt.
			  (1<<USIWM1)|(0<<USIWM0)|  			// Set USI in Two-wire mode. No USI Counter overflow prior
									    			// to first Start Condition (potential failure)
			  (1<<USICS1)|(0<<USICS0)|(0<<USICLK)| 	// Shift Register Clock Source = External, positive edge
			  (0<<USITC);
	USISR    = 0xF0;                                // Clear all flags and reset overflow counter


	// enable interrupts
	sei();

}

/* Detects the USI_TWI Start Condition and intialises the USI
 * for reception of the "TWI Address" packet.
 */

ISR(USI_START_VECTOR)
{
    unsigned char tmpUSISR;                                         // Temporary variable to store volatile
    tmpUSISR = USISR;                                               // Not necessary, but prevents warnings
// Set default starting conditions for new TWI package
    USI_TWI_Overflow_State = USI_SLAVE_CHECK_ADDRESS;
    DDR_USI  &= ~(1<<PORT_USI_SDA);                                 // Set SDA as input
    while ( (PIN_USI & (1<<PORT_USI_SCL)) & !(tmpUSISR & (1<<USIPF)) );   // Wait for SCL to go low to ensure the "Start Condition" has completed.
                                                                       // If a Stop condition arises then leave the interrupt to prevent waiting forever.
    USICR   =   (1<<USISIE)|(1<<USIOIE)|                            // Enable Overflow and Start Condition Interrupt. (Keep StartCondInt to detect RESTART)
                (1<<USIWM1)|(1<<USIWM0)|                            // Set USI in Two-wire mode.
                (1<<USICS1)|(0<<USICS0)|(0<<USICLK)|                // Shift Register Clock Source = External, positive edge
                (0<<USITC);
    USISR  =    (1<<USI_START_COND_INT)|(1<<USIOIF)|(1<<USIPF)|(1<<USIDC)|      // Clear flags
                (0x0<<USICNT0);                                     // Set USI to sample 8 bits i.e. count 16 external pin toggles.
}

/* Handles all the communication. Is disabled only when waiting
 * for new Start Condition.
 */
ISR(USI_OVERFLOW_VECTOR)
{
  unsigned char tmpUSIDR;


  switch (USI_TWI_Overflow_State) {
    // ---------- Address mode ----------
    // Check address and send ACK (and next USI_SLAVE_SEND_DATA) if OK, else reset USI.
    case USI_SLAVE_CHECK_ADDRESS:
      if( (( USIDR>>1 ) == TWI_slaveAddress) || ((USIDR == 0) && allowGenCall) ) {
    	// I am being addressed, check if for read or write
    	  if ( USIDR & 0x01 ){
    	  		i2cState = I2C_SLAVE_TX;
    	  		// ask application for the data to be sent
    	  		if(i2cSlaveTransmit && txBuffer){
    	  			// prepare buffer
    	  			bufferFlush(txBuffer);
    	  			if(i2cSlaveReceive && rxBuffer){
    	  				i2cSlaveReceive(rxBuffer);
    	  			}
    	  			i2cSlaveTransmit(txBuffer);
    	  		}
    	  		USI_TWI_Overflow_State =  USI_SLAVE_SEND_DATA;
    	  } else {
    			i2cState = I2C_SLAVE_RX;
    			// prepare receive buffer
    			bufferFlush(rxBuffer);
    		    USI_TWI_Overflow_State =  USI_SLAVE_REQUEST_DATA;
    	  }


    	// Ack the address
        SET_USI_TO_SEND_ACK();
      } else {
    	// Not my address so wait for next start
        SET_USI_TO_TWI_START_CONDITION_MODE();
      }
      break;

    // ----- Master write data mode ------
    // Check reply for previous send and goto USI_SLAVE_SEND_DATA if OK, else reset USI.
    case USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA:
      if ( USIDR ){
    	// If NACK, the master does not want more data.
        SET_USI_TO_TWI_START_CONDITION_MODE();
        return;
      }
      // From here we just drop straight into USI_SLAVE_SEND_DATA if the master sent an ACK
      // to send the next data byte

    // Copy data from buffer to USIDR and set USI to shift byte.
    // Next USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA
    case USI_SLAVE_SEND_DATA:

      // Get data from Buffer
      if (bufferGet(txBuffer, &tmpUSIDR)){
    	  // Send the next byte
    	  USIDR = tmpUSIDR;
      } else {
    	  // If the buffer is empty then:
          SET_USI_TO_TWI_START_CONDITION_MODE();
          return;
      }
      USI_TWI_Overflow_State = USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA;
      SET_USI_TO_SEND_DATA();
      break;

    // Set USI to sample reply from master. Next USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA
    case USI_SLAVE_REQUEST_REPLY_FROM_SEND_DATA:
      USI_TWI_Overflow_State = USI_SLAVE_CHECK_REPLY_FROM_SEND_DATA;
      SET_USI_TO_READ_ACK();
      break;

    // ----- Master read data mode ------
    // Set USI to sample data from master. Next USI_SLAVE_GET_DATA_AND_SEND_ACK.
    case USI_SLAVE_REQUEST_DATA:
      USI_TWI_Overflow_State = USI_SLAVE_GET_DATA_AND_SEND_ACK;
      SET_USI_TO_READ_DATA();
      break;

    // Copy data from USIDR and send ACK. Next USI_SLAVE_REQUEST_DATA
    case USI_SLAVE_GET_DATA_AND_SEND_ACK:
      // Put data into receive Buffer
      tmpUSIDR = USIDR;             // Not necessary, but prevents warnings
      if (rxBuffer)
    	  bufferPut(rxBuffer, tmpUSIDR);

      USI_TWI_Overflow_State = USI_SLAVE_REQUEST_DATA;
      SET_USI_TO_SEND_ACK();
      break;
  }
}

#else
// No hardware I2C slave support !
#endif

