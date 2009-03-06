/*
 * commands.c
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#include "commands.h"

void command_eval(const char *command) {
	if (!strcmp_P(command, PSTR(COMMAND_PHASE_COOLING))) {
		// Enable cooling
		relais_set(1);
		uart_puts_P(PSTR("\n\r => Cooling enabled.\n\r"));
	} else if (!strcmp_P(command, PSTR(COMMAND_PHASE_WARMING))) {
		// Disable cooling
		relais_set(0);
		uart_puts_P(PSTR("\n\r => Cooling disabled.\n\r"));
	} else {
		// Unknown command
		uart_puts_P(PSTR("\n\r => Unknown command: "));
		uart_puts(command);
		uart_puts_P(PSTR(CR));
	}
}
