/*
 * $Id: $
 * Revision History
 * ================
 * $Log: $
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
 *        File: timerNextPrescaler.c
 *  Created on: 30 Dec 2010
 *      Author: Clive Webster
 */
#include "../timer.h"

// The prescale factors for the different timers - as of schema version 2 these are now generated
//static const uint16_t  PROGMEM TimerPrescaleFactor[] 		= {0,1,8,   64,    256,1024};
//static const uint16_t  PROGMEM TimerRTCPrescaleFactor[] 	= {0,1,8,32,64,128,256,1024};

uint8_t timerGetPrescalerIndex(uint16_t prescale, const Timer* timer){

	const uint16_t *array = (const uint16_t *)pgm_read_word(&timer->pgm_divisors);
	const uint8_t count = pgm_read_word(array++);

	for(uint8_t i=0; i<count; i++){
		uint16_t one = pgm_read_word(array + i);
		if(one == prescale){
			return i;
		}
	}
	return 0;
}

uint16_t timerNextPrescaler(uint16_t prescaler,const Timer* timer){
	const uint16_t *array = (const uint16_t *)pgm_read_word(&timer->pgm_divisors);
	const uint8_t count = pgm_read_word(array++);

	for(uint8_t i=0; i<count; i++){
		uint16_t one = pgm_read_word(array + i);
		if(one > prescaler){
			return one;
		}
	}
	return 0;
}
