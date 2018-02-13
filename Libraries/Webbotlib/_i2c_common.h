/*
 * $Id: _i2c_common.h,v 1.6 2010/10/01 19:55:03 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: _i2c_common.h,v $
 * Revision 1.6  2010/10/01 19:55:03  clivewebster
 * Setting the bit rate only applies to a hardware master
 *
 * Revision 1.5  2010/09/08 18:30:06  clivewebster
 * Refactor common info to allow for i2c_slave
 *
 * Revision 1.4  2010/06/14 19:21:25  clivewebster
 * Add copyright license info
 *
 * Revision 1.3  2009/11/02 18:48:32  clivewebster
 * Added revision log
 *
 * ===========
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
 * _i2c_common.h
 *
 *  Created on: 10-Sep-2009
 *      Author: Clive Webster
 */

#ifndef _I2C_COMMON_H_
#define _I2C_COMMON_H_

#include "libdefs.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum{
	I2C_IDLE=0,
	I2C_BUSY,
	I2C_MASTER_TX,
	I2C_MASTER_RX,
	I2C_SLAVE_TX,
	I2C_SLAVE_RX
} i2cStateType;

extern volatile i2cStateType i2cState;


// ----- public methods: available to applications ------------

#ifdef BUILDING_LIBRARY

// defines and constants
#	define TWCR_CMD_MASK		0x0F
#	define TWSR_STATUS_MASK		0xF8

#	undef I2C_OVER_USI
#	undef I2C_HARDWARE

#	if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega128__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega128RFA1__) || defined(__AVR_ATmega2561__)
#		define I2C_HARDWARE
#		define SCL_PORT PORTD
#		define SCL_DDR  DDRD
#		define SCL_PIN  PD0
#		define SDA_PORT PORTD
#		define SDA_DDR  DDRD
#		define SDA_PIN  PD1
#	elif defined(__AVR_ATmega8__) || defined(__AVR_ATmega168__)
#		define I2C_HARDWARE
#		define SCL_PORT PORTC
#		define SCL_DDR  DDRC
#		define SCL_PIN  PC5
#		define SDA_PORT PORTC
#		define SDA_DDR  DDRC
#		define SDA_PIN  PC4
#	elif defined(__AVR_ATmega32__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega1284P__)
#		define I2C_HARDWARE
#		define SCL_PORT PORTC
#		define SCL_DDR  DDRC
#		define SCL_PIN  PC0
#		define SDA_PORT PORTC
#		define SDA_DDR  DDRC
#		define SDA_PIN  PC1
#	elif defined(__AVR_ATmega328P__)
#		define I2C_HARDWARE
#		define SCL_PORT PORTC
#		define SCL_DDR  DDRC
#		define SCL_PIN  PORTC5
#		define SDA_PORT PORTC
#		define SDA_DDR  DDRC
#		define SDA_PIN  PORTC4
#	elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#   	define I2C_OVER_USI
#		define DDR_USI             DDRB
#		define PORT_USI            PORTB
#		define PIN_USI             PINB
#		define PORT_USI_SDA        PORTB0
#		define PORT_USI_SCL        PORTB2
#		define PIN_USI_SDA         PINB0
#		define PIN_USI_SCL         PINB2
#		define USI_START_COND_INT  USISIF
#		define USI_START_VECTOR    USI_START_vect
#		define USI_OVERFLOW_VECTOR USI_OVF_vect
#	else
#error Dont know how to handle this processor
#	endif

#	if(!defined(I2C_HARDWARE))
# 		if(!defined(I2C_OVER_USI))
#error You must specify I2C_HARDWARE or I2C_OVER_USI otherwise no I2C will be added
#		endif
#	endif

#endif // BUILDING_LIBRARY

#ifdef __cplusplus
}
#endif

#endif /* _I2C_COMMON_H_ */
