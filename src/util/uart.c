#include "main.h"
#include "uart.h"
#include <util/delay.h>

#ifndef INTERRUPT_ENABLED
    void uart_init(void) {
        // UBRR setzen
        UBRRH = (uint8_t) (BAUD_PRESCALE >> 8);
        UBRRL = (uint8_t) (BAUD_PRESCALE);

        // UART Receiver und Transmitter anschalten
        UCSRB = (1 << RXEN) | (1 << TXEN);
        // URSEL for Register UCSRC selection,
        // UCSZ0 & 1 for data mode 8N1, asynchron
        UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
    }

    void uart_putc(unsigned char c) {
        //Warten, bis Senden moeglich ist
        while (!(UCSRA & (1 << UDRE)))
            ;

        //schreibt das Zeichen auf die Schnittstelle
        UDR = c;
    }

    void uart_puts(char *s) {
        while (*s) {
            uart_putc(*s);
            s++;
        }
        _delay_ms(10);
    }

    uint8_t uart_readc( void ) {
        return UDR;
    }

    unsigned char uart_getc( void ) {
      /* Wait for data to be received */
        while ((UCSRA & (1 << RXC)) == 0) {
            /* Blink led 4 as a 'polling' flag */
            PORTC &= ~(1<<4); // enable led 4
            _delay_ms(20); // wait
            PORTC |= (1<<4); // disable led 4
            _delay_ms(20); // wait
      }
      // enable led 2 as a 'received' flag
      PORTC &= ~(1<<2);
      /* Get and return received data from buffer */
      return UDR;
    }
#else /* INTERRUPT_ENABLED */
    // FIFO-Objekte und Puffer für die Ein- und Ausgabe
    #define BUFSIZE_IN  0x40
    uint8_t inbuf[BUFSIZE_IN];
    fifo_t infifo;
    #define BUFSIZE_OUT 0x40
    uint8_t outbuf[BUFSIZE_OUT];
    fifo_t outfifo;

    // Timeout Flag
    uint8_t timeout_flag = 0;

    void uart_init() {
        //uint8_t sreg = SREG;
        uint16_t ubrr = (uint16_t) ((uint32_t) F_CPU/(16*USART_BAUDRATE) - 1);

        // Interrupts kurz deaktivieren
        cli();

        // UBRR setzen
        UBRRH = (uint8_t) (ubrr>>8);
        UBRRL = (uint8_t) (ubrr);

        // UART Receiver und Transmitter anschalten, Receive-Interrupt aktivieren
        // Data mode 8N1, asynchron
        UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);
        UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);

        // Flush Receive-Buffer (entfernen evtl. vorhandener ungültiger Werte)
        do
        {
            // UDR auslesen (Wert wird nicht verwendet)
            UDR;
        }
        while (UCSRA & (1 << RXC));

        // Rücksetzen von Receive und Transmit Complete-Flags
        UCSRA = (1 << RXC) | (1 << TXC);

        // Global Interrupt-Flag wieder herstellen
        //SREG = sreg;

        // FIFOs für Ein- und Ausgabe initialisieren
        fifo_init (&infifo,   inbuf, BUFSIZE_IN);
        fifo_init (&outfifo, outbuf, BUFSIZE_OUT);

        // Interrupts global aktivieren
        sei();
    }

    /*************************
     * Interrupt-Routinen
     *************************/

    // Empfangene Zeichen werden in die Eingabgs-FIFO gespeichert und warten dort
    ISR (USART_RXC_vect)
    {
        _inline_fifo_put (&infifo, UDR);
    }

    // Ein Zeichen aus der Ausgabe-FIFO lesen und ausgeben
    // Ist das Zeichen fertig ausgegeben, wird ein neuer SIG_UART_DATA-IRQ getriggert
    // Ist die FIFO leer, deaktiviert die ISR ihren eigenen IRQ.
    ISR (USART_UDRE_vect)
    {
        if (outfifo.count > 0)
           UDR = _inline_fifo_get (&outfifo);
        else
            UCSRB &= ~(1 << UDRIE);
    }

    /*************************
     * Ein-/Ausgabe
     *************************/

    int uart_putc (const uint8_t c) {
        int ret = fifo_put (&outfifo, c);

        UCSRB |= (1 << UDRIE);

        return ret;
    }

    int uart_getc_nowait () {
        return fifo_get_nowait (&infifo);
    }

    uint8_t uart_getc_wait (int16_t timeout) {
        timeout_flag = 0;
        return fifo_get_wait (&infifo, timeout);
    }

    // Einen 0-terminierten String übertragen.
    void uart_puts (const char *s) {

        while (*s) {
            uart_putc (*s);
            s++;
        }
        _delay_ms(10);
    }

    // Einen 0-terminierten String senden, der im Flash steht.
    void uart_puts_P (PGM_P s) {
        while (1)
        {
            unsigned char c = pgm_read_byte (s);
            s++;
            if ('\0' == c)
                break;
            uart_putc (c);
        }
        _delay_ms(10);
    }

    // Einen '\n' terminierten String empfangen
    void uart_gets(char* Buffer, char firstChar, uint8_t MaxLen, int16_t timeout) {
        uint8_t NextChar;
        uint8_t StringLen = 0;

        // Print command prompt
        uart_puts_P(PSTR(PROMPT));

        if (firstChar != '\0') {
            // Take given char as starting character
            NextChar = firstChar;
        } else {
            // Await and get next char
            NextChar = uart_getc_wait(timeout);
        }
        // Collect chars until line break or array full
        while (NextChar != '\n' && NextChar != '\r' && NextChar != 0xFF
                && StringLen < MaxLen - 1) {
            *Buffer++ = NextChar;
            StringLen++;
            uart_putc(NextChar);
            // Read next char
            NextChar = uart_getc_wait(timeout);
        }
        uart_puts_P(PSTR(CR));
        // Add '\0' to finish string
        *Buffer = '\0';
        // Check timeout
        if (NextChar == 0xFF) {
            timeout_flag = 1;
        }
    }

    uint8_t inline uart_get_timeout(void) {
        return timeout_flag;
    }
#endif /* INTERRUPT_ENABLED */
