/*
 * main.h
 *
 *  Created on: 03.03.2009
 *      Author: chh
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include "util/myavr.h"
#include "util/uart.h"
#include "util/rtc.h"
#include "util/ds18s20.h"
#include "util/random.h"

void led_chaser(void);
uint8_t receive(char* line, uint8_t maxlen);

#endif /* MAIN_H_ */
