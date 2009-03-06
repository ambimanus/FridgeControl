/*
 * rtc.h
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#ifndef RTC_H_
#define RTC_H_

#include "main.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define DEBOUNCE	250L		// debounce clock (256Hz = 4msec)

void rtc_init(void);
uint8_t rtc_hasChanged(void);
void rtc_clearFlag(void);
uint32_t rtc_getTime(void);

#endif /* RTC_H_ */
