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
	// Real time counter
	uint32_t *volatile rtc;
	// RTC change flag
	uint8_t *volatile rtc_flag;

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
	rtc_init(rtc, rtc_flag);

	// Welcome
	uart_puts_P(PSTR("\n\r **** FridgeControl ****\n\r"));

	/****************************
	 * Main loop				*
	 ****************************/
	while (1) {
		// Check time
		if (*rtc_flag) {
			*rtc_flag = 0;
			uart_puts_P(PSTR(("Time: ")));
			sprintf(buf_s, "%lu\n\r", *rtc);
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
