/*
 * commands.c
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#include "commands.h"

void command_eval(const char *command) {
    if (!strcmp_P(command, PSTR(COMMAND_HELLO))) {
        // Print welcome
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" Hello, this is **** FridgeControl ****"));
        uart_puts_P(PSTR(CR));
        command_eval(COMMAND_HELP);
        command_eval(COMMAND_STATUS);
    } else if (!strcmp_P(command, PSTR(COMMAND_HELP))) {
        // Print help
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" => Available commands:"));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > help\tThis text."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > status\tPrint controller status."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > time\tPrint the current timestamp."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > indy\tPlay Indiana Jones Theme."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > imperial\tPlay Imperial March Theme."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > cooling\tSwitch to mode 'cooling'."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > warming\tSwitch to mode 'warming'."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > set t_min\tRedefine T_MIN."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > set t_max\tRedefine T_MAX."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" => LEDs explanation:"));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > LED 0\tClock: flashes each second."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > LED 1\tCommunication: flashes while receiving."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > LED 2\tFridge: flashes while cooling."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
    } else if (!strcmp_P(command, PSTR(COMMAND_STATUS))) {
        char buf_s[32];
        // Print status
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" => Controller status:"));
        uart_puts_P(PSTR(CR));
        // Print time
        uart_puts_P(PSTR("  > Current timestamp = "));
        sprintf(buf_s, "%lu", rtc_getTime());
        uart_puts(buf_s);
        uart_puts_P(PSTR(CR));
        // Print device values
        uart_puts_P(PSTR("  > T_current = "));
        if (SIMULATE == 1) {
            dtostrf(simulator_get_temperature(), -1, 3, &buf_s[0]);
        } else {
            dtostrf(ds18s20_get_temperature(), -1, 3, &buf_s[0]);
        }
        uart_puts(buf_s);
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > T_MIN = "));
        dtostrf(basecontroller_get_t_min(), -1, 3, &buf_s[0]);
        uart_puts(buf_s);
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > T_MAX = "));
        dtostrf(basecontroller_get_t_max(), -1, 3, &buf_s[0]);
        uart_puts(buf_s);
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > tau_cooling = "));
        dtostrf(basecontroller_get_tau_cooling(), -1, 3, &buf_s[0]);
        uart_puts(buf_s);
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > tau_warming = "));
        dtostrf(basecontroller_get_tau_warming(), -1, 3, &buf_s[0]);
        uart_puts(buf_s);
        uart_puts_P(PSTR(CR));
//      // Print relais status
//      uart_puts_P(PSTR("  > Cooling device is "));
//      if (relais_get_state() == RELAIS_ON) {
//          uart_puts_P(PSTR("on."));
//      } else {
//          uart_puts_P(PSTR("off."));
//      }
//      uart_puts_P(PSTR(CR));
        // Print basecontroller status
        uart_puts_P(PSTR("  > BaseController is in Mode "));
        switch (basecontroller_get_state()) {
        case BASE_COOLING: uart_puts_P(PSTR("'cooling'.")); break;
        case BASE_WARMING: uart_puts_P(PSTR("'warming'.")); break;
        }
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
    } else if (!strcmp_P(command, PSTR(COMMAND_TIME))) {
        // Show time
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" => (time="));
        char buf_s[32];
        sprintf(buf_s, "%lu)", rtc_getTime());
        uart_puts(buf_s);
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
    } else if (!strcmp_P(command, PSTR(COMMAND_INDY))) {
        // Play 'indy' tune
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" => Playing Indiana Jones Theme!'."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
        speaker_tune_indy();
    } else if (!strcmp_P(command, PSTR(COMMAND_IMPERIAL))) {
        // Play 'imperial' tune
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" => Playing Imperial March Theme!'."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
        speaker_tune_imperial();
    } else if (!strcmp_P(command, PSTR(COMMAND_COOLING))) {
        // Enable cooling
        basecontroller_begin_cooling();
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" => Mode 'cooling'."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
    } else if (!strcmp_P(command, PSTR(COMMAND_WARMING))) {
        // Disable cooling
        basecontroller_begin_warming();
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" => Mode 'warming'."));
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
    } else if (!strcmp_P(command, PSTR(COMMAND_SET_T_MIN))) {
        // Redefine T_MIN
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > T_MIN currently is "));
        char buf_s[8];
        sprintf(buf_s, "%0#.1f", (double) basecontroller_get_t_min());
        uart_puts(buf_s);
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > Enter new T_MIN: "));
        while (1) {
            // Read line
            uart_gets(buf_s, '\0', sizeof(buf_s), USART_TIMEOUT);
            // Check timeout
            if (uart_get_timeout()) {
                uart_puts_P(PSTR(CR));
                uart_puts_P(PSTR(" => Input timeout! Command "));
                uart_puts_P(PSTR(COMMAND_SET_T_MIN));
                uart_puts_P(PSTR(" aborted, try again."));
                uart_puts_P(PSTR(CR));
                uart_puts_P(PSTR(CR));
                return;
            }
            // Evaluate received line
            float t_min = -200.0f;
            t_min = atof(buf_s);
            if (t_min == -200.0f) {
                uart_puts_P(PSTR(CR));
                uart_puts_P(PSTR(" => Parameter not recognized."));
            } else {
                basecontroller_set_t_min(t_min);
                uart_puts_P(PSTR(CR));
                uart_puts_P(PSTR(" => T_MIN changed to "));
                sprintf(buf_s, "%0#.1f", (double) basecontroller_get_t_min());
                uart_puts(buf_s);
                break;
            }
        }
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
    } else if (!strcmp_P(command, PSTR(COMMAND_SET_T_MAX))) {
        // Redefine T_MIN
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > T_MAX currently is "));
        char buf_s[8];
        sprintf(buf_s, "%0#.1f", (double) basecontroller_get_t_max());
        uart_puts(buf_s);
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR("  > Enter new T_MAX: "));
        while (1) {
            // Read line
            uart_gets(buf_s, '\0', sizeof(buf_s), USART_TIMEOUT);
            // Check timeout
            if (uart_get_timeout()) {
                uart_puts_P(PSTR(CR));
                uart_puts_P(PSTR(" => Input timeout! Command "));
                uart_puts_P(PSTR(COMMAND_SET_T_MAX));
                uart_puts_P(PSTR(" aborted, try again."));
                uart_puts_P(PSTR(CR));
                uart_puts_P(PSTR(CR));
                return;
            }
            // Evaluate received line
            float t_max = -200.0f;
            t_max = atof(buf_s);
            if (t_max == -200.0f) {
                uart_puts_P(PSTR(CR));
                uart_puts_P(PSTR(" => Parameter not recognized."));
            } else {
                basecontroller_set_t_max(t_max);
                uart_puts_P(PSTR(CR));
                uart_puts_P(PSTR(" => T_MAX changed to "));
                sprintf(buf_s, "%0#.1f", (double) basecontroller_get_t_max());
                uart_puts(buf_s);
                break;
            }
        }
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
    } else {
        // Unknown command
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(" => Unknown command: "));
        uart_puts(command);
        uart_puts_P(PSTR(CR));
        uart_puts_P(PSTR(CR));
    }
}