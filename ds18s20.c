/*
 * ds18b20.c
 *
 *  Created on: 26.01.2009
 *      Author: chh
 */

#include "ds18s20.h"

float current_temperature = T_MIN;

void ds18s20_init() {
	DDRA &= 0x00;	// Set all port pins as input,
	PORTA |= 0xff;	// activate internal pullups to avoid floating input pins,
	wL;				// but not for the data line, have external pullup of 4.7k required for DS1820.
}

uint8_t ds18s20_wReset(void) {

	/* For timings see an162.pdf */

	// Return var
	uint8_t present;


	// Init condition:
//	wOut;
//	wH;
//	wIn;
//	_delay_ms(1);


	// Pull line low for 480us (reset pulse)
	wOut;
	wL;
	_delay_us(480);
	// Release bus
	wIn;
	// Wait ((15~60)+x)us for rising edge
	_delay_us(60+10);
	// Read presence
	present = wR;
	// Wait ((60~240)-x)us (presence pulse)
	_delay_us(240-10);

	// 0 == device present, 1 == fail
	return present;
}

void ds18s20_wTxbit(uint8_t bit) {

	/* For timings see an162.pdf */

	// Init write time slot by pulling low
	wOut;
	wL;
	if (bit) {
		// If bit==1, hold low for (1~15)us
		_delay_us(5);
		// Release bus
		wIn;
		// Wait (60~120)us for read time slot to finish
		_delay_us(60);
	} else {
		// If bit==0, hold low for (60~120)us
		_delay_us(60);
		// Release bus
		wIn;
		// Recover for at least 1us
		_delay_us(1);
	}
}

uint8_t ds18s20_wRxbit(void) {
	uint8_t data;

	// Init read time slot by pulling low for at least 1us
	wOut;
	wL;
	_delay_us(1);
	// Release bus
	wIn;
	// Wait 15us-1us=14us
	_delay_us(14);
	// Read line
	data = wR;
	// Wait 60us-15us=45us for read time slot to finish
	_delay_us(45);

	return data;
}

void ds18s20_wTxbyte(uint8_t data) {
	uint8_t i;

	for (i = 0; i < 8; i++) {
		ds18s20_wTxbit(data & 0x01);
		data >>= 1;
	}
}

uint8_t ds18s20_wRxbyte(void) {
	uint8_t i, bit, data;

	data = 0;
	for (i = 0; i < 8; i++) {
		bit = ds18s20_wRxbit();
		data |= bit << i;
	}

	return data;
}

uint8_t ds18s20_reset() {
	if(DS18S20_DEBUG) uart_puts_P(PSTR(" Sensor reset: "));
	uint8_t ret = ds18s20_wReset();
	if(DS18S20_DEBUG) {
		char buf_s[32];
		sprintf(buf_s, "%d\n\r", ret);
		uart_puts(buf_s);
		if (ret == 0) {
			uart_puts_P(PSTR("   => successful.\n\r"));
		} else {
			uart_puts_P(PSTR("   => fail.\n\r\n\r"));
		}
	}
	return ret;
}

void ds18s20_read_rom() {
	if (ds18s20_wReset() == 0) {
		if (DS18S20_DEBUG) uart_puts_P(PSTR(" Reading ROM Code\n\r"));
		ds18s20_wTxbyte(READ_ROM);
		char rom[8];
		for (int i = 0; i < 8; i++) {
			rom[i] = ds18s20_wRxbyte();
		}
		char buf_s[32];
		sprintf(buf_s, "   => %02X %02X %02X %02X %02X %02X %02X %02X\n\r", rom[7],
				rom[6], rom[5], rom[4], rom[3], rom[2], rom[1], rom[0]);
		if (DS18S20_DEBUG) uart_puts(buf_s);
	}
}

uint8_t ds18s20_start_measure() {
	// reset
	if (ds18s20_wReset() == 0) {
		// skip rom
		if (DS18S20_DEBUG) uart_puts_P(PSTR(" Skipping ROM\n\r"));
		ds18s20_wTxbyte(SKIP_ROM);
		// start measurement
		if (DS18S20_DEBUG) uart_puts_P(PSTR(" Starting measurement\n\r"));
		ds18s20_wTxbyte(CONVERT_T);
		return 0;
	}
	return 1;
}

float ds18s20_read_temperature() {
	char get[9];
	char temp_lsb, temp_msb;
	int k;
	// reset
	if (ds18s20_wReset() == 0) {
		// skip rom
		if (DS18S20_DEBUG) uart_puts_P(PSTR(" Skipping ROM\n\r"));
		ds18s20_wTxbyte(SKIP_ROM);
		// init read scratchpad
		if (DS18S20_DEBUG) uart_puts_P(PSTR(" Init scratchpad reading\n\r"));
		ds18s20_wTxbyte(READ_SCRATCHPAD);
		// read scratchpad
		if (DS18S20_DEBUG) uart_puts_P(PSTR(" Reading scratchpad\n\r"));
		for (k = 0; k < 9; k++) {
			get[k] = ds18s20_wRxbyte();
		}
		// print data
		if (DS18S20_DEBUG) {
			char buf_s[32];
			sprintf(buf_s,
					"   => %02X %02X %02X %02X %02X %02X %02X %02X %02X\n\r",
					get[8], get[7], get[6], get[5], get[4], get[3], get[2],
					get[1], get[0]);
			uart_puts(buf_s);
		}
		if (DS18S20_DEBUG) uart_puts_P(PSTR(" Interpreting scratchpad\n\r"));
		// MS Byte: Sign byte + lsbit
		temp_msb = get[1];
		// LS Byte: Temp data plus lsb
		temp_lsb = get[0];
		// shift to get whole degree
		if (temp_msb <= 0x80) {
			temp_lsb = (temp_lsb / 2);
		}
		// mask all but the sign bit
		temp_msb = temp_msb & 0x80;
		// twos complement
		if (temp_msb >= 0x80) {
			temp_lsb = (~temp_lsb) + 1;
		}
		// shift to get whole degree
		if (temp_msb >= 0x80) {
			temp_lsb = (temp_lsb / 2);
		}
		// add sign bit
		if (temp_msb >= 0x80) {
			temp_lsb = ((-1) * temp_lsb);
		}
		// calculate 0.1°C precision
		current_temperature = (float) temp_lsb - 0.25f + ((float) (get[7]
				- get[6]) / (float) get[7]);
		// print data
		if (DS18S20_DEBUG) {
			uart_puts_P(PSTR("   => Temperature: "));
			char buf_s[32];
			sprintf(buf_s, "%0#.1f °C\n\r", (double) current_temperature);
			uart_puts(buf_s);
		}
	}
	return current_temperature;
}

float ds18s20_get_temperature() {
	return current_temperature;
}
