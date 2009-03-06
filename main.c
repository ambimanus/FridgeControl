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
	// 'receive' flag
	int rx = -1;
	// Line buffer (64 chars)
	char line[64];
	// Print buffer (32 chars)
	char buf_s[32];
	// Current temperature
	float temperature;

	/****************************
	 * Init						*
	 ****************************/
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

	/****************************
	 * Startup					*
	****************************/
	// Welcome message
	uart_puts_P(PSTR("\n\r **** FridgeControl ****\n\r"));
	uart_puts_P(PSTR("\n\r Starting fridge control unit...\n\r"));
	// Perform first measurement
	ds18s20_startMeasure();
	_delay_ms(2000);
	temperature = ds18s20_readTemperature();
	// Start fridge control unit
	basecontroller_init(temperature, rtc_getTime());
	uart_puts_P(PSTR("   => up and running.\n\r\n\r"));

	/****************************
	 * Main loop				*
	 ****************************/
	while (1) {
		// Check time
		if (rtc_hasChanged()) {
			rtc_clearFlag();
			uart_puts_P(PSTR(("Time: ")));
			sprintf(buf_s, "%lu\n\r", rtc_getTime());
			uart_puts(buf_s);
		}
		// Check for received character
		rx = uart_getc_nowait();
		// If char received, read whole line
		if (rx != -1) {
			// Enable led 1 as 'receiving' flag
			led_set(1,1);
			// Read line
			uart_gets(line, rx, sizeof(line));
			// Evaluate received line
			command_eval(line);
			// Disable led 1
			led_set(1, 0);
		}
		// Wait till next loop
		_delay_ms(100);
	}
}
