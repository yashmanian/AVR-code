/*
 * $Id: $
 * Revision History
 * ================
 * $Log: $
 * ================
 *
 * Copyright (C) 2011 Clive Webster (webbot@webbot.org.uk)
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
 *        File: GP2wide.h
 *  Created on: 14 May 2011
 *      Author: Clive Webster
 */
#ifndef GP2_WIDE_H_
#define GP2_WIDE_H_


#include "GP2.h"

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

/*
 * GP2Y3A002K0F
 * Pin 7 is ground
 * Pin 8 is the supply voltage
 */
// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2Y3A002K0F;

typedef struct s_Sharp_GP2Y3A002K0F{
	DISTANCE distance;		// Holds the 'average' distance across all sensors
	ADC_CHANNEL adcPin; 	// The adc pin used to read value (connect to pin 6)
	const IOPin* led[5];	// The iopins used to select each led (connect to pins 1..5)
	DISTANCE_TYPE led_cm[5];// The distance value from each led
	const IOPin* vin;		// The iopin used to turn on the device (connect to pin 9)
	uint8_t pinNo;			// The current pin being sampled
}Sharp_GP2Y3A002K0F;

// Create the sensor
#define MAKE_Sharp_GP2Y3A002K0F(adcPin, led1,led2,led3,led4,led5, vin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y3A002K0F), \
	adcPin, \
	{led1, led2, led3, led4, led5}, \
	{0, 0, 0, 0, 0}, \
	vin, 0}

/*
 * GP2Y3A003K0F
 * Pin 7 is ground
 * Pin 8 is the supply voltage
 */
// The class used to read a raw value
extern const DISTANCE_CLASS c_Sharp_GP2Y3A003K0F;

typedef struct s_Sharp_GP2Y3A003K0F{
	DISTANCE distance;		// Holds the 'average' distance across all sensors
	ADC_CHANNEL adcPin; 	// The adc pin used to read value (connect to pin 6)
	const IOPin* led[5];	// The iopins used to select each led (connect to pins 1..5)
	DISTANCE_TYPE led_cm[5];// The distance value from each led
	const IOPin* vin;		// The iopin used to turn on the device (connect to pin 9)
	uint8_t pinNo;			// The current pin being sampled
}Sharp_GP2Y3A003K0F;

// Create the sensor
#define MAKE_Sharp_GP2Y3A003K0F(adcPin, led1,led2,led3,led4,led5, vin) { \
	MAKE_DISTANCE_SENSOR(c_Sharp_GP2Y3A003K0F), \
	adcPin, \
	{led1, led2, led3, led4, led5}, \
	{0, 0, 0, 0, 0}, \
	vin, 0}

#ifdef __cplusplus
}
/* ===================== C++ Code ===============================================*/
class Sharp5_wide : public Distance{
public:
	Sharp5_wide(DISTANCE* cstr) : Distance(cstr){
	}

	virtual DISTANCE_TYPE getBeam(uint8_t num) = 0;
	virtual uint8_t getBeamNumber(void) const = 0;
};

class Sharp5_GP2Y3A002K0F : public Sharp5_wide{
public:
	Sharp5_GP2Y3A002K0F(Sharp_GP2Y3A002K0F* c) : Sharp5_wide(&c->distance){
		m_cstr = c;
	}

	virtual DISTANCE_TYPE getBeam(uint8_t num){
		return (num<5) ? m_cstr->led_cm[num] : 0;
	}

	virtual uint8_t getBeamNumber(void) const {
		return m_cstr->pinNo;
	}

private:
	Sharp_GP2Y3A002K0F* m_cstr;
};

class Sharp5_GP2Y3A003K0F : public Sharp5_wide{
public:
	Sharp5_GP2Y3A003K0F(Sharp_GP2Y3A003K0F* c) : Sharp5_wide(&c->distance){
		m_cstr = c;
	}
	virtual DISTANCE_TYPE getBeam(uint8_t num){
		return (num<5) ? m_cstr->led_cm[num] : 0;
	}
	virtual uint8_t getBeamNumber(void) const {
		return m_cstr->pinNo;
	}
private:
	Sharp_GP2Y3A003K0F* m_cstr;
};

#endif


#endif
