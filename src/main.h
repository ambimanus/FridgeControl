/*
 * main.h
 *
 *  Created on: 03.03.2009
 *      Author: chh
 */

#ifndef MAIN_H_
#define MAIN_H_

#define INFO
#define SIMULATE            0
#if SIMULATE
#define REPORT_INTERVAL     1           // report temperature every x seconds
#else
#define REPORT_INTERVAL     60          // report temperature every x seconds
#endif

#include <stdio.h>
#include <stdlib.h>
#include "myavr.h"
#include "uart.h"
#include "rtc.h"
#include "commands.h"
#include "ds18s20.h"
#include "basecontroller.h"
#include "simulator.h"

#endif /* MAIN_H_ */
