/*
 * main.h
 *
 *  Created on: 03.03.2009
 *      Author: chh
 */

#ifndef MAIN_H_
#define MAIN_H_

//#define F_CPU				16000000UL
#define F_CPU				16000348UL	// deviation calculated by rtc
#define INFO
#define SIMULATE			0
#if SIMULATE
#define REPORT_INTERVAL		1			// report temperature every x seconds
#else
#define REPORT_INTERVAL		60			// report temperature every x seconds
#endif
#define CR					"\r"
#define ROUND_INT_8(d)		( (int8_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_INT_16(d)		( (int16_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_INT_32(d)		( (int32_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_UINT_8(d)		( (uint8_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_UINT_16(d)	( (uint16_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )
#define ROUND_UINT_32(d)	( (uint32_t) ((d) + ((d) > 0 ? 0.5 : -0.5)) )

#include "myavr.h"
#include "random.h"
#include "uart.h"
#include "rtc.h"
#include "commands.h"
#include "ds18s20.h"
#include "basecontroller.h"
#include "dsc.h"
#include "simulator.h"
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#endif /* MAIN_H_ */
