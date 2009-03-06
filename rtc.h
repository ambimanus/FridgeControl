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

void rtc_init(uint32_t *volatile s, uint8_t *volatile f);

#endif /* RTC_H_ */
