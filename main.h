/*
 * main.h
 *
 *  Created on: 03.03.2009
 *      Author: chh
 */

#ifndef MAIN_H_
#define MAIN_H_

//#define F_CPU	16000000UL
#define F_CPU	16000348UL	// deviation calculated by rtc
#define CR		"\n\r"
#define INFO

#include "myavr.h"
#include "random.h"
#include "uart.h"
#include "rtc.h"
#include "commands.h"
#include "ds18s20.h"
#include "basecontroller.h"
#include "dsc.h"
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#endif /* MAIN_H_ */
