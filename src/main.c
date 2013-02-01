/*
 * main.c
 *
 *  Created on: 05.03.2009
 *      Author: chh
 */

#include "main.h"
//#include "util/lcd.c"

#define DB4      3
#define DB5      2
#define DB6      1
#define DB7      0
#define LCD_E    5

#define KS0073_EXTENDED_FUNCTION_REGISTER_ON  0x24   /* |0|010|0100 4-bit mode extension-bit RE = 1 */
#define KS0073_EXTENDED_FUNCTION_REGISTER_OFF 0x20   /* |0|000|1001 4 lines mode */
#define KS0073_4LINES_MODE                    0x09   /* |0|001|0000 4-bit mode, extension-bit RE = 0 */

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
    // Init LCD
    DDRB |= 0xFF; // set port as output
    PORTB &= 0x00; // set all pins low
    delay_us(20000);
    port_b_setpin(8, 0); // GND
    port_b_setpin(9, 1); // VCC
    //port_b_setpin(4, 1); // light
    delay_us(20000);
    
    PORTB = 0x03;
    PORTB |= (1<<LCD_E);
    PORTB &= ~(1<<LCD_E);
    delay_us(4100);
    PORTB = 0x03;
    PORTB |= (1<<LCD_E);
    PORTB &= ~(1<<LCD_E);
    delay_us(100);
    PORTB = 0x03;
    PORTB |= (1<<LCD_E);
    PORTB &= ~(1<<LCD_E);
    delay_us(100);
    
    PORTB = 0x02;
    PORTB |= (1<<LCD_E);
    PORTB &= ~(1<<LCD_E);
    delay_us(100);
    
    lcd_command(KS0073_EXTENDED_FUNCTION_REGISTER_ON);
    lcd_command(KS0073_4LINES_MODE);
    lcd_command(KS0073_EXTENDED_FUNCTION_REGISTER_OFF);
    
    lcd_command(LCD_DISP_OFF);              /* display off                  */
    lcd_clrscr();                           /* display clear                */ 
    lcd_command(LCD_MODE_DEFAULT);          /* set entry mode               */
    lcd_command(0x0F);                      /* display/cursor control       */
    //lcd_command(LCD_DISP_ON);              /* display off                  */
    
    //lcd_init(0x24);
    //lcd_init(0x0F);

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
            led_chaser();
        }
        // Check buttons
        if (button == 0) {
            button = button_read();
            if (button > 0) {
                char buf_s[32];
                uart_puts_P(PSTR("Button "));
                sprintf(buf_s, "%d", button);
                uart_puts(buf_s);
                uart_puts_P(PSTR(" pressed."));
                uart_puts_P(PSTR(CR));
                
                // port_b_setpin(4, 1);
            }
        } else {
            uint8_t btn = button_read();
            if (btn == 0) {
                char buf_s[32];
                uart_puts_P(PSTR("Button "));
                sprintf(buf_s, "%d", button);
                uart_puts(buf_s);
                uart_puts_P(PSTR(" released."));
                uart_puts_P(PSTR(CR));
                
                // port_b_setpin(4, 0);
            }
            button = btn;
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

void led_chaser(void) {
    for(int i = 0; i < 8; i++) {
        // Enable led i
        led_set(i, 1);
        _delay_ms(10);
        // Disable led 0
        led_set(i, 0);
        _delay_ms(5);
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
