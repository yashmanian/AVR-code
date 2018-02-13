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
 *        File: timerSetMode.c
 *  Created on: 3 Jan 2011
 *      Author: Clive Webster
 */
#include "../timer.h"

// Index is the TIMER_MODE and the result is the value to write to the WGM bits or 255 if the mode isn't supported
//								TIMER_MODE:     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
//static const uint8_t PROGMEM __3bit_modes[] = { 0,  1,255,255,  2,  3,255,255,255,255,255,  5,255,255,255,  7};
//static const uint8_t PROGMEM __2bit_modes[] = { 0,  1,255,255,  2,  3};
// eg for 3 WGM bits: to set Mode=11 then the WGM bits are set to '5'


/*
 * Convert the mode into the wgm bits or 255 if the mode is not supported
 */
static uint8_t timerModeToWGM(const Timer* timer, TIMER_MODE mode){
	uint8_t wgm;
	uint8_t numModes = pgm_read_byte(&timer->pgm_modetowgmLen);
	if (mode < numModes){
		const uint8_t* tbl = (const uint8_t*)pgm_read_word(&timer->pgm_modetowgm);
		wgm = pgm_read_byte(&tbl[mode]);
	} else {
		wgm = 255;
	}
	return wgm;
}

boolean timerIsModeSupported(const Timer* timer, TIMER_MODE mode){
	uint8_t wgm = timerModeToWGM(timer, mode);
	return (wgm==255) ? FALSE : TRUE;
}
/* -------------------------------------------------------
//  Set the mode for a given timer
//  This will disconnect the output OCx pins on all
//  channels and will remove any callbacks on the channels
------------------------------------------------------- */
void timerSetMode(const Timer* timer, TIMER_MODE mode){
	if(mode == TIMER_MODE_NORMAL || timerGetMode(timer)!=mode){
		uint8_t wgm = timerModeToWGM(timer, mode);

		if(wgm != 255){
			// It is capable of the mode

			// Save the new mode
			timerGetData(timer)->mode = mode;

			timerOff(timer);	// turn timer off whilst changing the mode

			const PORT_MASK* pm = &timer->pgm_wgm[0];
			for(int i=0; i<4;i++,pm++){
				PORT port= pgm_read_word(pm->port);

				if(port==0){
					break;
				}

				if(wgm & 1){
					__portMaskSet(pm);
				}else{
					__portMaskClear(pm);
				}
				wgm>>=1;
			}

			// re-initialise the timer :- set count to 0 and reset the prescaler to turn it back on
			PORT counter = pgm_read_word(&timer->pgm_counter);
			if(timerIs16bit(timer)){
				_SFR_MEM16(counter) = 0;
			}else{
				_SFR_MEM8(counter) = 0;
			}
			timerSetPrescaler(timer,timerGetPrescaler(timer));
		}else{
			// Timer is not able to do this mode
			setError(TIMER_MODE_NOT_SUPPORTED);
		}
	}
}
