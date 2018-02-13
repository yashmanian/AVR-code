/*
 * $Id: libdefs.h,v 1.18 2010/07/01 23:52:02 clivewebster Exp $
 *
 * Revision History
 * ================
 * $Log: libdefs.h,v $
 * Revision 1.18  2010/07/01 23:52:02  clivewebster
 * Ignore CRTITICAL_SECTION under Windows target
 *
 * Revision 1.17  2010/06/15 00:48:59  clivewebster
 * Add copyright license info
 *
 * Revision 1.16  2010/06/07 18:28:18  clivewebster
 * Timers no longer have a counter for when the overflow or compare interrupts occur. If you want one then implement your own in the service routine. Done to make interrupts faster.
 *
 * Revision 1.15  2010/04/25 03:26:23  clivewebster
 * Use PI from <math.h>
 *
 * Revision 1.14  2010/04/02 15:58:30  clivewebster
 * *** empty log message ***
 *
 * Revision 1.13  2010/03/24 19:53:12  clivewebster
 * Add macro definitions for KB and MB
 *
 * Revision 1.12  2010/03/07 20:12:33  clivewebster
 * *** empty log message ***
 *
 * Revision 1.11  2010/02/21 19:53:23  clivewebster
 * Make timerGetMode into an inline function rather than querying the timer hardware. Much faster.
 *
 * Revision 1.10  2010/02/09 15:03:50  clivewebster
 * Define MAKE_READER
 *
 * Revision 1.9  2010/01/24 14:35:32  clivewebster
 * *** empty log message ***
 *
 * Revision 1.8  2009/11/02 19:00:09  clivewebster
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
 * libdefs.h
 *
 *  Created on: 11-Mar-2009
 *      Author: Clive Webster
 */

#ifndef LIBDEFS_H_
#define LIBDEFS_H_

#ifdef __cplusplus
/* ===================== C Code ===============================================*/
extern "C" {
#endif

#if defined(_WINDOWS_)
	#define PROGMEM
	#include <memory.h>
	#define pgm_read_byte(addr) *addr
	#define pgm_read_word(addr) *addr
	#define memcpy_P(a,b,c) memcpy(a,b,c);
	#define prog_char
	typedef unsigned long uint32_t;
	typedef unsigned int uint16_t;
	typedef unsigned char uint8_t;
	typedef signed char int8_t;
	typedef int int16_t;
	#define PSTR(s) s
	#define FALSE 0
	#define TRUE  1
	#define __inline__
	#define  null ((void*)0)
	#define MAKE_WRITER(name) uint8_t name(uint8_t byte)

#else
	// include common WinAVR stuff
	#include <inttypes.h>
	#include <stdint.h>
	#include <avr/common.h>
	#include <avr/pgmspace.h>
	#include <avr/interrupt.h>
	#include <stdio.h>

	// For C++ redefine the PROGMEM macro to stop compiler warnings
	#ifdef __cplusplus
	#undef PROGMEM
	#define PROGMEM __attribute__((section(".progmem.something")))
	#endif

#endif

// create a type for boolean
typedef int8_t boolean;

// create a type to store timer us ticks
// maximum is 0xFFFFFFFF or 4294967295us = 4294 seconds or 71 minutes
typedef uint32_t TICK_COUNT;

// Create a type to store a loop count
typedef uint32_t LOOP_COUNT;

// Define some hardware data types
typedef uint16_t PORT; // A port number eg _SFR_MEM_ADDR(PORTA)
typedef uint8_t PIN;  // A pin numnber eg 0...7


#ifndef null
  // Define a value for 'null'
#ifndef __cplusplus
#define null ((void *)0)
#else   /* C++ */
#define null 0
#endif  /* C++ */
#endif


#ifndef _WINDOWS_
static __inline__ uint8_t __iCliRetVal(void){ cli(); return 1; }
static __inline__ uint8_t __iSeiRetVal(void){ sei(); return 1; }
static __inline__ void __iRestore(const  uint8_t *__s){ SREG = *__s; __asm__ volatile ("" ::: "memory");}
#define CRITICAL_SECTION for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = SREG, __ToDo = __iCliRetVal(); __ToDo ; __ToDo = 0 )
#define INTERRUPTABLE_SECTION for ( uint8_t sreg_save __attribute__((__cleanup__(__iRestore))) = SREG, __ToDo = __iSeiRetVal(); __ToDo ; __ToDo = 0 )
#else
#define CRITICAL_SECTION
#endif

/*
#ifndef CRITICAL_SECTION_START
#ifdef _WINDOWS_
#define CRITICAL_SECTION_START
#define CRITICAL_SECTION_END
#else
#define CRITICAL_SECTION_START	unsigned char _sreg = SREG; cli()
#define CRITICAL_SECTION_END    SREG = _sreg
#endif
#endif

#ifndef INTERRUPTABLE_SECTION_START
#ifdef _WINDOWS_
#define INTERRUPTABLE_SECTION_START
#define INTERRUPTABLE_SECTION_END
#else
#define INTERRUPTABLE_SECTION_START	unsigned char _sreg = SREG; sei()
#define INTERRUPTABLE_SECTION_END    SREG = _sreg
#endif
#endif
*/


// Read a byte from ROM or RAM
#define READMEMBYTE(rom,char_ptr)	((rom)?(pgm_read_byte(char_ptr)):(*char_ptr))

// --------------- avrlibdefs.h ------------------------
// Code compatibility to new AVR-libc
// outb(), inb(), inw(), outw(), BV(), sbi(), cbi(), sei(), cli()
#ifndef outb
	#define	outb(addr, data)	addr = (data)
#endif
#ifndef inb
	#define	inb(addr)			(addr)
#endif
#ifndef outw
	#define	outw(addr, data)	addr = (data)
#endif
#ifndef inw
	#define	inw(addr)			(addr)
#endif

#ifndef BV
	#define BV(bit)			(1<<(bit))
#endif
#ifndef cbi
	#define cbi(reg,bit)	reg &= ~(BV(bit))
#endif
#ifndef sbi
	#define sbi(reg,bit)	reg |= (BV(bit))
#endif
#ifndef cli
	#define cli()			__asm__ __volatile__ ("cli" ::)
#endif
#ifndef sei
	#define sei()			__asm__ __volatile__ ("sei" ::)
#endif
#ifndef nop
	#define nop()			__asm__ __volatile__ ("nop" ::)
#endif



// use this for packed structures
// (this is seldom necessary on an 8-bit architecture like AVR,
//  but can assist in code portability to AVR)
#define GNUC_PACKED __attribute__((packed))


// MIN/MAX/ABS macros
#define MIN(a,b)			(((a)<(b))?(a):(b))
#define MAX(a,b)			(((a)>(b))?(a):(b))
#define ABS(x)				(((x)>0)?(x):(-(x)))

// Clamp a value to be in a given range
#define CLAMP(val, min, max) MAX(MIN(val,max),min)

// constants
// use M_PI from <math.h> instead
//#define PI		3.14159265359


// ----------------- avrlibtypes.h -------------------------------
// Include standard files from AVRLIB
#ifndef WIN32
	// true/false defines
	#define FALSE	0
	#define TRUE	-1
	#define false	FALSE
	#define true	TRUE
#endif

// datatype definitions macros
//typedef uint8_t  u08;
//typedef  int8_t  s08;
//typedef uint16_t u16;
//typedef  int16_t s16;
//typedef uint32_t u32;
//typedef  int32_t s32;
//typedef unsigned long long u64;
//typedef   signed long long s64;

// maximum value that can be held
// by unsigned data types (8,16,32bits)
#define MAX_U08	255U
#define MAX_U16	65535UL
#define MAX_U32	4294967295UL

// maximum values that can be held
// by signed data types (8,16,32bits)
#define MIN_S08	-128
#define MAX_S08	127
#define MIN_S16	-32768
#define MAX_S16	32767
#define MIN_S32	-2147483648
#define MAX_S32	2147483647

//#ifndef WIN32
	// more type redefinitions
//	typedef uint8_t   BOOL;
//	typedef uint8_t	  BYTE;
//	typedef uint16_t  WORD;
//	typedef uint32_t  DWORD;

//	typedef BYTE  UCHAR;
//	typedef WORD  UINT;
//	typedef WORD  USHORT;
//	typedef DWORD ULONG;

//	typedef int8_t	CHAR;
//	typedef int16_t	INT;
//	typedef int32_t	LONG;
//#endif

typedef int8_t ERROR_CODE;

// A type to store an ADC channel number ouutput by a2d:ADC_NUMBER_TO_CHANNEL
typedef uint8_t ADC_CHANNEL;

typedef uint32_t BAUD_RATE;

typedef uint8_t PERCENTAGE;



#define UNUSED_PORT _SFR_MEM8(0X00)

// Define a baud rate that represents 'the highest possible' - I know it looks like a weird value
// but it is trapped and the baud rate registe pair is set to '1'
#define BAUD_RATE_MAX ((BAUD_RATE)MAX_U32)

#if !defined(_WINDOWS_)
#if defined(BUILDING_LIBRARY)
extern const uint32_t cpu_speed;
extern const uint16_t cpu_speed_div_1000;
extern const uint16_t cpu_speed_div_1000000;
#else
const uint32_t cpu_speed = F_CPU;
const uint16_t cpu_speed_div_1000 = F_CPU / 1000U;
const uint16_t cpu_speed_div_1000000 = F_CPU / 1000000U;
#endif
#endif


typedef struct s_portmask{
	const PORT		port;
	const PIN 		mask;
} PORT_MASK;

/******************************************************

	 Define a structure to hold the definition
	 of an I/O Port

*******************************************************/

typedef struct s_ioport{
	const PORT		port;
	const PORT		ddr;
	const PORT		pin;
}IOPort;
/******************************************************

	 Define a structure to hold the definition
	 of an I/O Pin ie a Port and a Pin number

*******************************************************/
typedef struct s_IOPin {
	const IOPort* port;	// The port
	PIN pin;			// The ie BV(PB2)
} IOPin;



/* ---------------------------------------------------------------------------

Define the name for each of the possible timer modes 0...16

BOTTOM is always a TCNTn = 0
MAX is always a TCNTn = 0xFF for an 8 bit timer or 0xFFFF for a 16 bit timer
TOP can vary per mode
---------------------------------------------------------------------------- */
typedef enum {
	/*
	 * Mode 0: Normal
	 *  Timer counts repeatedly from BOTTOM to MAX.
	 *  TOV<Timer> will also be set when it wraps from MAX to BOTTOM
	 */
	TIMER_MODE_NORMAL=0,

	/*
	 * Mode 1:
	 */
	TIMER_MODE_PWM8_PHASE_CORRECT,
	TIMER_MODE_PWM9_PHASE_CORRECT,
	TIMER_MODE_PWM10_PHASE_CORRECT,

	/*
	 * Mode 4: Clear Timer on Compare Match using OCR<Timer>A register value
	 *  Timer counts repeatedly from BOTTOM to TOP.
	 *  TOP is the value stored in the OCR<Timer>A register
	 *  An interrupt can be generated when going to BOTTOM by setting OCF<Timer>A bit
	 *  The COM<Timer>A bits can be set to toggle the OC<Timer>A pin but OC<Timer>A must be set as an output pin
	 *  Frequency on OC<Timer>A pin = (F_CPU) div ( 2 * prescaler * (1 + OCR<Timer>A))
	 *  If TOP = MAX then TOV<Timer> will also be set when it wraps from MAX to BOTTOM
	 */
	TIMER_MODE_CTC_OCR,

	/*
	 * Mode 5: 8 bit fast pwm
	 *  Timer counts repeatedly from BOTTOM to TOP (=0xFF).
	 *  The OCR<Timer><Channel> register is used to vary the duty mode by using values from BOTTOM to TOP.
	 *  The COM<Timer><Channel> bits can be set to provide either inverted or non-inverted output on pin OCR<Timer><Channel>
	 *  TOV<Timer> bit is set on reaching TOP
	 *  PWM Freq = (F_CPU) div (prescaler * (1 + TOP)) = (F_CPU) div (prescaler * 0x100)
	 */
	TIMER_MODE_PWM8_FAST,

	/*
	 * Mode 6: 9 bit fast pwm
	 *  Timer counts repeatedly from BOTTOM to TOP (=0x1FF).
	 *  The OCR<Timer><Channel> register is used to vary the duty mode by using values from BOTTOM to TOP.
	 *  The COM<Timer><Channel> bits can be set to provide either inverted or non-inverted output on pin OCR<Timer><Channel>
	 *  TOV<Timer> bit is set on reaching TOP
	 *  PWM Freq = (F_CPU) div (prescaler * (1 + TOP)) = (F_CPU) div (prescaler * 0x200)
	 */
	TIMER_MODE_PWM9_FAST,

	/*
	 * Mode 7: 10 bit fast pwm
	 *  Timer counts repeatedly from BOTTOM to TOP (=0x3FF).
	 *  The OCR<Timer><Channel> register is used to vary the duty mode by using values from BOTTOM to TOP.
	 *  The COM<Timer><Channel> bits can be set to provide either inverted or non-inverted output on pin OCR<Timer><Channel>
	 *  TOV<Timer> bit is set on reaching TOP
	 *  PWM Freq = (F_CPU) div (prescaler * (1 + TOP)) = (F_CPU) div (prescaler * 0x400)
	 */
	TIMER_MODE_PWM10_FAST,

	TIMER_MODE_PWM_PHASE_FREQ_ICR,
	TIMER_MODE_PWM_PHASE_FREQ_OCR,

	TIMER_MODE_PWM_PHASE_CORRECT_ICR,
	TIMER_MODE_PWM_PHASE_CORRECT_OCR,

	/*
	 * Mode 12: Clear Timer on Compare Match using ICR<Timer> register value
	 *  Timer counts repeatedly from BOTTOM to TOP.
	 *  TOP is the value stored in the ICR<Timer> register
	 *  An interrupt can be generated when going to BOTTOM by setting ICF<Timer> bit
	 *  The COM<Timer><Channel> bits can be set to toggle the OC<Timer><Channel> pin but OC<Timer><Channel> pin must be set as an output pin
	 *  Frequency = OC<Timer><Channel> pin = (F_CPU) div ( 2 * prescaler * (1 + OCR<Timer><Channel>))
	 *  If TOP = MAX then TOV will also be set when it wraps from MAX to BOTTOM
	 */
	TIMER_MODE_CTC_ICR,

	// Reserved - do not use
	TIMER_MODE_13_RESVD,

	/*
	 * Mode 14: fast pwm using ICR<Timer> to set frequency
	 *  Timer counts repeatedly from BOTTOM to TOP (= contents of ICR<Timer> register).
	 *  The OCR<Timer><Channel> register is used to vary the duty mode by using values from BOTTOM to TOP.
	 *  The COM<Timer><Channel> bits can be set to provide either inverted or non-inverted output on pin OCR<Timer><Channel>
	 *  TOV<Timer> bit is set on reaching TOP
	 *  ICF<Timer> bit is set on reaching TOP
	 *
	 *  PWM Freq = (F_CPU) div (prescaler * (1 + TOP)) = (F_CPU) div (prescaler * (1 + ICR<Timer>) )
	 *
	 *  Good if PWM freq never changes. But bad if it changes as ICR<Timer> register is not double buffered
	 *  and so if changing ICR<Timer> register during interrupts then timing can be wrong.
	 *  If PWM frequency changes then consider Mode 15.
	 */
	TIMER_MODE_PWM_FAST_ICR,

	/*
	 * Mode 15: fast pwm using OCR<Timer>A to set frequency
	 *  Timer counts repeatedly from BOTTOM to TOP (= contents of OCR<Timer>A register).
	 *  The OCR<Timer><Channel> register is used to vary the duty mode by using values from BOTTOM to TOP.
	 *  The COM<Timer><Channel> bits can be set to provide either inverted or non-inverted output on pin OCR<Timer><Channel>
	 *  TOV<Timer> bit is set on reaching TOP
	 *  OC<Timer>A bit is set on reaching TOP
	 *
	 *  PWM Freq = (F_CPU) div (prescaler * (1 + TOP)) = (F_CPU) div (prescaler * (1 + OCR<Timer>A) )
	 *
	 *  Better than Mode 14 if PWM frequency is changed in interrupt routine as the OCR<Timer>A register is double buffered
	 */
	TIMER_MODE_PWM_FAST_OCR
} TIMER_MODE;
#define MAX_MODES 16

/*
	Declare a datatype that can store a list of modes, one bit per TIMER_MODE.
*/
typedef uint16_t TIMER_MODES;

// The set of PWM modes
#define PWM_MODES ((TIMER_MODES)( \
	BV(TIMER_MODE_PWM8_PHASE_CORRECT) | \
	BV(TIMER_MODE_PWM9_PHASE_CORRECT) | \
	BV(TIMER_MODE_PWM10_PHASE_CORRECT) | \
	BV(TIMER_MODE_PWM8_FAST) | \
	BV(TIMER_MODE_PWM9_FAST) | \
	BV(TIMER_MODE_PWM10_FAST) | \
	BV(TIMER_MODE_PWM_PHASE_FREQ_ICR) | \
	BV(TIMER_MODE_PWM_PHASE_FREQ_OCR) | \
	BV(TIMER_MODE_PWM_PHASE_CORRECT_ICR) | \
	BV(TIMER_MODE_PWM_PHASE_CORRECT_OCR) | \
	BV(TIMER_MODE_PWM_FAST_ICR) | \
	BV(TIMER_MODE_PWM_FAST_OCR) \
))

// The set of ICR modes
#define ICR_MODES ((TIMER_MODES)( \
	BV(TIMER_MODE_PWM_PHASE_FREQ_ICR) | \
	BV(TIMER_MODE_PWM_PHASE_CORRECT_ICR) | \
	BV(TIMER_MODE_CTC_ICR) | \
	BV(TIMER_MODE_PWM_FAST_ICR)  \
))

// The set of OCR modes
#define OCR_MODES ((TIMER_MODES)( \
	BV(TIMER_MODE_CTC_OCR) | \
	BV(TIMER_MODE_PWM_PHASE_FREQ_OCR) | \
	BV(TIMER_MODE_PWM_PHASE_CORRECT_OCR) | \
	BV(TIMER_MODE_PWM_FAST_OCR)  \
))

#define modeIsPWM(mode) ( ((TIMER_MODES)(BV(mode))) & PWM_MODES)
#define modeIsICR(mode) ( ((TIMER_MODES)(BV(mode))) & ICR_MODES)
#define modeIsOCR(mode) ( ((TIMER_MODES)(BV(mode))) & OCR_MODES)

/*
#define TIMER_NO_MODES 0

// Define bits if all modes are supported
#define TIMER_ALL_MODES ((TIMER_MODES)( \
	BV(TIMER_MODE_NORMAL)|\
	BV(TIMER_MODE_PWM8_PHASE_CORRECT)|\
	BV(TIMER_MODE_PWM9_PHASE_CORRECT)|\
	BV(TIMER_MODE_PWM10_PHASE_CORRECT)|\
	BV(TIMER_MODE_CTC_OCR)|\
	BV(TIMER_MODE_PWM8_FAST)|\
	BV(TIMER_MODE_PWM9_FAST)|\
	BV(TIMER_MODE_PWM10_FAST)|\
	BV(TIMER_MODE_PWM_PHASE_FREQ_ICR)|\
	BV(TIMER_MODE_PWM_PHASE_FREQ_OCR)|\
	BV(TIMER_MODE_PWM_PHASE_CORRECT_ICR)|\
	BV(TIMER_MODE_PWM_PHASE_CORRECT_OCR)|\
	BV(TIMER_MODE_CTC_ICR)|\
	BV(TIMER_MODE_PWM_FAST_ICR)|\
	BV(TIMER_MODE_PWM_FAST_OCR)\
))

// Define bits for less capable timers

#define TIMER_3BIT_MODES (BV(TIMER_MODE_NORMAL)| BV(TIMER_MODE_PWM8_PHASE_CORRECT)|BV(TIMER_MODE_CTC_OCR)| \
		BV(TIMER_MODE_PWM8_FAST)|BV(TIMER_MODE_PWM_PHASE_CORRECT_OCR)|BV(TIMER_MODE_PWM_FAST_OCR))

#define TIMER_2BIT_MODES (BV(TIMER_MODE_NORMAL)| BV(TIMER_MODE_PWM8_PHASE_CORRECT)|BV(TIMER_MODE_CTC_OCR)| \
		BV(TIMER_MODE_PWM8_FAST))
*/

/**
	Define the constant info for a timer compare unit
	This information will be held in PROGMEM
*/
typedef struct s_timer_compare{
	void* const 		pgm_data;					// The address of the volatile data in SRAM of type TimerDataCompare*
	const uint8_t		pgm_timer;					// The timer number 0...NUMBER_OF_TIMERS
	PORT_MASK			pgm_intenable;				// The port and bit mask that is used to turn on/off compare interrupts
	const PORT			pgm_threshold;				// The port to set the compare value, For a sixteen bit port should be the low one ie OCR1AL
	PORT_MASK			pgm_intpending;				// The port and bit mask that is used to indicate an interrupt is pending
	PORT_MASK			pgm_com;					// The port and bit number used to store COMxy0
	const IOPin * /*PROGMEM*/	pgm_pwm;				// The pwm pin if any
} TimerCompare;

// Define a callback for TimerCompare events
typedef void (*TimerCompareCallback)(const TimerCompare *timer_compare, void* data);

/**
	Define the volatile info for a timer compare unit
	This information will be held in Flash SRAM
*/
typedef struct s_timer_compare_data{
	TimerCompareCallback compare_callback; 	// The callback function for this timer	when it compare is met
	void* 				compare_data;		// The data passed to the callback routine
//    volatile TICK_COUNT overflow; 			// The timer overflow value incremented via interrupts. Thats why its volatile
} TimerDataCompare;


#define MAKE_TIMER_COMPARE_DATA() {0,null/*,0*/}
#define MAKE_TIMER_COMPARE(data,timer,port,mask,threshold, intport,intmask, comport,combit,pwm) \
	{&data,timer, {_SFR_MEM_ADDR(port),BV(mask)},_SFR_MEM_ADDR(threshold), \
	{_SFR_MEM_ADDR(intport),BV(intmask)}, \
	{_SFR_MEM_ADDR(comport),combit}, \
	pwm \
	}

/**
	Define the constant info for a timer
	This information will be held in PROGMEM
*/
typedef struct s_timer{
	void* const   	pgm_data;			// The address of the volatile data in SRAM of type TimerData*
	const PORT	  	pgm_counter;		// The port that counts up from the prescaler eg TCNT0
	PORT_MASK	  	pgm_prescaler;		// The port/mask used for the prescaler
	const boolean 	pgm_sixteenBit;		// TRUE if it is a 16 bit timer
	const uint8_t 	pgm_numCompare;		// The number of compare channels
//	const TIMER_MODES pgm_modes;		// The modes supported by this timer - Version 1
	const uint8_t	pgm_modetowgmLen;	// The length of the pgm_modetowgm array - allows lists to overlap
	const uint8_t*	pgm_modetowgm;		// Address of table to convert mode into wgm value
	PORT_MASK 		pgm_wgm[4];			// Up to 4 WGM bits
	const TimerCompare* pgm_compares;    // The compare units
	PORT_MASK	  	pgm_overflowint;	// The port/mask for overflow interrupt enable
	PORT_MASK	  	pgm_overflowreq;	// The port/mask used to indicate an overflow interrupt is pending
	const PORT		pgm_icr;			// Input compare register
	PORT_MASK	  	pgm_captureint;		// The port/mask for capture interrupt enable
	PORT_MASK	  	pgm_capturereq;		// The port/mask used to indicate a capture interrupt is pending
	PORT_MASK	  	pgm_captureedge;	// The port/mask used to select rising falling edge for capture input
	const IOPin*	pgm_capturepin;		// The pin used for input capture, or null if not available
	const uint16_t* pgm_divisors;		// List of valid prescaler values for this timer
} Timer;

// Declare a data type for the timer interrrupt callback function
typedef void (*TimerCallback)(const Timer *timer, void* data);


/**
	Define the volatile info for a timer.
	This information will be held in Flash SRAM
*/
typedef struct s_timer_data{
	uint16_t			prescale_value;		// The value used in the prescaler. use timerSetPrecaler to change
//    volatile TICK_COUNT overflow; 			// The timer overflow value incremented via interrupts. Thats why its volatile
	TimerCallback	  	overflow_callback; 	// The callback function for this timer	when its counter overflow
	void* 				overflow_data;		// The data passed to the callback overflow routine
	TimerCallback		capture_callback;	// The callback function for this timer	when its capture pin is raised
	void* 				capture_data;		// The data passed to the callback capture routine
	TIMER_MODE			mode;				// The current mode
} TimerData;

#define MAKE_TIMER_DATA(prescale) {prescale,/*0,*/null,null,null,null,TIMER_MODE_NORMAL}
#define MAKE_TIMER(data, counter,prescalerPort, prescalerMask,sixteenBit, modeslen, modestbl, \
	wgm0port, wgm0mask, wgm1port, wgm1mask, wgm2port, wgm2mask, wgm3port, wgm3mask, \
    compares,ovrintport,ovrintmask,ovrreqport,ovrreqmask,icr, \
    capintport,capintmask,capreqport,capreqmask, capedgeport,capedgemask,\
    incapin, \
    divisors \
	) \
	{&data, _SFR_MEM_ADDR(counter), \
		{_SFR_MEM_ADDR(prescalerPort),prescalerMask}, \
	sixteenBit, sizeof(compares)/sizeof(TimerCompare),\
	modeslen, modestbl, \
    	{ \
			{_SFR_MEM_ADDR(wgm0port),BV(wgm0mask)}, \
			{_SFR_MEM_ADDR(wgm1port),BV(wgm1mask)}, \
			{_SFR_MEM_ADDR(wgm2port),BV(wgm2mask)}, \
			{_SFR_MEM_ADDR(wgm3port),BV(wgm3mask)}  \
    	},\
    	compares,\
	{_SFR_MEM_ADDR(ovrintport),BV(ovrintmask)},\
	{_SFR_MEM_ADDR(ovrreqport),BV(ovrreqmask)}, \
	_SFR_MEM_ADDR(icr),\
	{_SFR_MEM_ADDR(capintport),BV(capintmask)},\
	{_SFR_MEM_ADDR(capreqport),BV(capreqmask)}, \
	{_SFR_MEM_ADDR(capedgeport),BV(capedgemask)}, \
	incapin, \
	divisors \
	}

// Define the signature of code that is the destination of an rprintf output
typedef int (*Writer)(uint8_t byte);
#define MAKE_WRITER(name) int name(uint8_t byte)

typedef int (*Reader)(void);
#define MAKE_READER(name) int name(void)


static __inline__ uint16_t get_uint16(const void* buf,size_t offset){
	return(*((const uint16_t*)(  ((const uint8_t*)buf) + offset)) );
}
static __inline__ int16_t get_int16(const void* buf,size_t offset){
	return(*((const int16_t*)(  ((const uint8_t*)buf) + offset)) );
}
static __inline__ uint32_t get_uint32(const void* buf,size_t offset){
	return(*((const uint32_t*)(  ((const uint8_t*)buf) + offset)));
}
static __inline__ void set_uint16(void* buf,size_t offset,uint16_t data){
	*((uint16_t*)( ((uint8_t*)buf) + offset)) = data;
}

static __inline__ void set_uint32(void* buf,size_t offset,uint32_t data){
	*((uint32_t*)( ((uint8_t*)buf) + offset)) = data;
}

#define KB (1024U)

// 1024*1024 = 1048576
#define MB (1048576UL)

// The datatype for specify an address within an EEPROM
typedef uint32_t EEPROM_ADDR;


#ifdef __cplusplus
}
// ========== C++ code ==========================
#define cin   ((Stream&)(*((Stream*)stdin)))
#define cout  ((Stream&)(*((Stream*)stdout)))
#define cerr  ((Stream&)(*((Stream*)stderr)))

#endif

#endif /* LIBDEFS_H_ */