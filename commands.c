/*
 * commands.c
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#include "commands.h"

void command_eval(const char *command) {
	if (!strcmp_P(command, PSTR(COMMAND_HELP))) {
		// Print help
		uart_puts_P(PSTR("\n\r => Available commands:\n\r"));
		_delay_ms(10); // Wait for transmission complete
		uart_puts_P(PSTR("  > help\tThis text.\n\r"));
		_delay_ms(10); // Wait for transmission complete
		uart_puts_P(PSTR("  > time\tPrints the current timestamp.\n\r"));
		_delay_ms(10); // Wait for transmission complete
		uart_puts_P(PSTR("  > cooling\tSwitches to 'cooling' mode.\n\r"));
		_delay_ms(10); // Wait for transmission complete
		uart_puts_P(PSTR("  > warming\tSwitches to 'warming' mode.\n\r"));
		_delay_ms(10); // Wait for transmission complete
		uart_puts_P(PSTR(CR));
	} else if (!strcmp_P(command, PSTR(COMMAND_TIME))) {
		// Show time
		uart_puts_P(PSTR("\n\r => (time="));
		char buf_s[32];
		sprintf(buf_s, "%lu)\n\r", rtc_getTime());
		uart_puts(buf_s);
	} else if (!strcmp_P(command, PSTR(COMMAND_COOLING))) {
		// Enable cooling
		basecontroller_begin_cooling(ds18s20_get_temperature(), T_MIN);
		uart_puts_P(PSTR("\n\r => Mode 'cooling'.\n\r"));
	} else if (!strcmp_P(command, PSTR(COMMAND_WARMING))) {
		// Disable cooling
		basecontroller_begin_warming(ds18s20_get_temperature(), T_MAX);
		uart_puts_P(PSTR("\n\r => Mode 'warming'.\n\r"));
	} else {
		// Unknown command
		uart_puts_P(PSTR("\n\r => Unknown command: "));
		uart_puts(command);
		uart_puts_P(PSTR(CR));
	}
}
