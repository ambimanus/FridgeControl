/*
 * main.c
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#include "main.h"

int main(void) {

	/****************************
	 * Vars						*
	 ****************************/
	// random seed
	uint16_t seed = random_get_seed();
	// 'receive' flag
	int rx = -1;
	// Line buffer (64 chars)
	char line[64];
	// Current temperature
	float temperature;
	// temperature report counter
	uint8_t reportcounter = -1;

	/****************************
	 * Init						*
	 ****************************/
	if (SIMULATE) {
		// Init simulator
		simulator_init();
	} else {
		// Init ds18s20
		ds18s20_init();
	}
	// Init LEDs
	led_init();
	// Init speaker
	speaker_init();
	// Init UART
	uart_init();
	// Init relais
	relais_init();
	// Init clock
	rtc_init();
	// Init rng
	srand(seed);
	srandom(seed);

	/****************************
	 * Startup					*
	****************************/
	// Welcome message
	uart_puts_P(PSTR(" Starting fridge control unit..."));
	if (SIMULATE == 1) {
		uart_puts_P(PSTR(" (simulation mode)"));
		// Start fridge control unit
		basecontroller_init(T_MIN_INIT, rtc_getTime());
		temperature = simulator_get_temperature();
	} else {
		// Perform first measurement
		ds18s20_start_measure();
		_delay_ms(2000);
		temperature = ds18s20_read_temperature();
		// Start fridge control unit
		basecontroller_init(temperature, rtc_getTime());
	}
	uart_puts_P(PSTR("   => ready."));
	uart_puts_P(PSTR(CR));
	command_eval(COMMAND_HELLO);

	/****************************
	 * Main loop				*
	 ****************************/
	while (1) {
		// Disable led 0
		led_set(0, 0);
		// Check time
		if (rtc_hasChanged()) {
			// One second elapsed, clear flag
			rtc_clearFlag();
			// Flash led 0
			led_set(0, 1);
			// read current temperature
			if (SIMULATE == 1) {
				temperature = simulator_get_temperature();
			} else {
				temperature = ds18s20_read_temperature();
			}
			// Increase report counter
			reportcounter++;
			// Report temperature every x seconds
			if (reportcounter == REPORT_INTERVAL) {
				reportcounter = 0;
				char buf_s[32];
				sprintf(buf_s, "#\t%lu", rtc_getTime());
				uart_puts(buf_s);
				uart_puts_P(PSTR("\t"));
				sprintf(buf_s, "%0#.1f", (double) temperature);
				uart_puts(buf_s);
				uart_puts_P(PSTR(CR));
			}
			uint8_t ret;
			if (SIMULATE == 1) {
				ret = 0;
			} else {
				// start next measure
				ret = ds18s20_start_measure();
			}
			if (ret) {
				command_eval(COMMAND_TIME);
				uart_puts_P(PSTR("    No response from sensor."));
				uart_puts_P(PSTR(CR));
				uart_puts_P(PSTR(CR));
			} else {
				// poll fridge controller
				basecontroller_poll(temperature, rtc_getTime());
				// poll dsc
				dsc_poll(temperature, rtc_getTime());
			}
		}
		// Check for received character
		rx = uart_getc_nowait();
		// If char received, read whole line
		if (rx != -1) {
			// Enable led 1 as 'receiving' flag
			led_set(1,1);
			// Read line
			uart_gets(line, rx, sizeof(line), USART_TIMEOUT);
			// Check timeout
			if (uart_get_timeout()) {
				uart_puts_P(PSTR("   => Input timeout! Try again."));
				uart_puts_P(PSTR(CR));
				uart_puts_P(PSTR(CR));
			} else {
				// Evaluate received line
				command_eval(line);
			}
			// Disable led 1
			led_set(1, 0);
		}
		// Wait till next loop
		_delay_ms(100);
	}
}
