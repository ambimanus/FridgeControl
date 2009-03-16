/*
 * ds18b20.h
 *
 *  Created on: 26.01.2009
 *      Author: chh
 */

#include "main.h"

#ifndef DS18S20_H_
#define DS18S20_H_

// debug flag
#define DS18S20_DEBUG	0

// set pin mask
#define wmask	(0x00)
// set 1-wire pin for output
#define wOut	(DDRA |= (1<<wmask))
// set 1-wire pin for input (bus released & pull-up on)
//#define wIn	(DDRA &= ~(1<<wmask)); (PORTA |= (1<<wmask))
// set 1-wire pin for input (bus released & pull-up off)
//#define wIn		(DDRA &= ~(1<<wmask)); (PORTA &= ~(1<<wmask))
// set 1-wire pin for input (bus released)
#define wIn		(DDRA &= ~(1<<wmask))
// set 1-wire bus high
#define wH		(PORTA |= (1<<wmask))
// set 1-wire bus low
#define wL		(PORTA &= ~(1<<wmask))
// get 1-wire bus input (bus released)
#define wR		(PINA & (1<<wmask))

/* ROM COMMANDS */
#define SEARCH_ROM			0xF0
#define READ_ROM			0x33
#define MATCH_ROM			0x55
#define SKIP_ROM			0xCC
#define ALARM_SEARCH		0xEC
/* FUNCTION COMMANDS */
#define CONVERT_T			0x44
#define WRITE_SCRATCHPAD	0x4E
#define READ_SCRATCHPAD		0xBE
#define COPY_SCRATCHPAD		0x48
#define RECALL_E2			0xB8
#define READ_POWER_SUPPLY	0xB4

//uint8_t ds18s20_wReset(void);
//void ds18s20_wTxbit(uint8_t bit);
//uint8_t ds18s20_wRxbit(void);
//void ds18s20_wTxbyte(uint8_t data);
//uint8_t ds18s20_wRxbyte(void);
void ds18s20_init(void);
uint8_t ds18s20_reset(void);
void ds18s20_read_rom(void);
uint8_t ds18s20_start_measure(void);
float ds18s20_read_temperature(void);
float ds18s20_get_temperature(void);

#endif /* DS18S20_H_ */
