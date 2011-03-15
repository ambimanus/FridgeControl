/*
 * myuart.h
 *
 *  Created on: 04.03.2009
 *      Author: chh
 */

#ifndef MYUART_H_
#define MYUART_H_

#define USART_TIMEOUT       30
#define USART_BAUDRATE      38400UL
//#define USART_BAUDRATE 9600UL
#define BAUD_PRESCALE       (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define INTERRUPT_ENABLED

#ifndef INTERRUPT_ENABLED
    #include <avr/io.h>

    void uart_init(void);
    void uart_putc(unsigned char c);
    void uart_puts(char *s);
    unsigned char uart_getc( void );
    uint8_t uart_readc( void );
#else /* INTERRUPT_ENABLED */
    #include <avr/io.h>
    #include <avr/pgmspace.h>
    #include <avr/interrupt.h>
    #include "fifo.h"

    extern void uart_init(void);
    extern int uart_putc (const uint8_t);
    extern uint8_t uart_getc_wait(int16_t timeout);
    extern int     uart_getc_nowait(void);
    // Einen 0-terminierten String übertragen.
    extern void uart_puts (const char *s);
    // Einen 0-terminierten String senden, der im Flash steht.
    extern void uart_puts_P (PGM_P s);
    // Einen '\n' oder '\r' terminierten String empfangen
    void uart_gets( char* Buffer, char firstChar, uint8_t MaxLen, int16_t timeout );
    // Get timeout-flag of last uart_get*_wait()
    uint8_t uart_get_timeout(void);

    static inline void uart_flush(void)
    {
        while (UCSRB & (1 << UDRIE));
    }
#endif /* INTERRUPT_ENABLED */

#endif /* MYUART_H_ */
