/*
 * main.c
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#include "main.h"

int main(void) {
    /****************************
     * Vars                     *
     ****************************/
    uint8_t button = 0;
    /****************************
     * Init                     *
     ****************************/
    // Init clock
    rtc_init();
    // Init LEDs
    led_init();
    // Init speaker
    speaker_init();
    // Init UART
    uart_init();

    /****************************
     * Startup                  *
    ****************************/
    // Welcome message
    uart_puts_P(PSTR("Hello World!"));
    uart_puts_P(PSTR(CR));
    char buf_s[32];
    uart_puts_P(PSTR("I was switched on "));
    sprintf(buf_s, "%d", rtc_getTime());
    uart_puts(buf_s);
    uart_puts_P(PSTR(" seconds ago."));
    uart_puts_P(PSTR(CR));

    /****************************
     * Main loop                *
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
        }
        // Check buttons
        button = button_read();
        if (button > 0) {
            char buf_s[32];
            uart_puts_P(PSTR("Button "));
            sprintf(buf_s, "%d", button);
            uart_puts(buf_s);
            uart_puts_P(PSTR(" pressed."));
            uart_puts_P(PSTR(CR));
        }
        // Receive data from UART
        char line[64];
        if (receive(line, sizeof(line))) {
            uart_puts_P(PSTR("Received:"));
            uart_puts_P(PSTR(CR));
            uart_puts(line);
            uart_puts_P(PSTR(CR));
        }
        // Wait till next loop
        _delay_ms(100);
    }
}

/**
 * This function tries to read data from UART into the given buffer.
 * It returns 1 on success, 0 otherwise.
 */
uint8_t receive(char* line, uint8_t maxlen) {
    // 'receive' flag
    int rx = -1;
    // Check for received character
    rx = uart_getc_nowait();
    // If char received, read whole line
    if (rx != -1) {
        // Enable led 1 as 'receiving' flag
        led_set(1,1);
        // Read line
        uart_gets(line, rx, maxlen, USART_TIMEOUT);
        // Disable led 1
        led_set(1, 0);
        // Check timeout
        if (uart_get_timeout()) {
            uart_puts_P(PSTR("   => Input timeout! Try again."));
            uart_puts_P(PSTR(CR));
            uart_puts_P(PSTR(CR));
            return 0;
        }
        return 1;
    }
    return 0;
}
