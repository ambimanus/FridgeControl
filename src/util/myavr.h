#ifndef MYAVR_H_
#define MYAVR_H_

//#define F_CPU             16000000UL
#define F_CPU               16000348UL  // deviation calculated by rtc

//#include "main.h"
#include <util/delay.h>
#include <avr/io.h>

/****************************
 * Ports                    *
 ****************************/
void port_a_setpin(const uint8_t pin, const uint8_t high);
void port_b_setpin(const uint8_t pin, const uint8_t high);
void port_c_setpin(const uint8_t pin, const uint8_t high);
void port_d_setpin(const uint8_t pin, const uint8_t high);

/****************************
 * Relais                   *
 ****************************/
#define RELAIS_DDR      DDRD
#define RELAIS_PORT     PORTD
#define RELAIS_PIN      4
#define RELAIS_ON       1
#define RELAIS_OFF      0

void relais_init(void);
void relais_test(void);
void relais_set(uint8_t enable);
uint8_t relais_get_state(void);

/****************************
 * LEDs                     *
 ****************************/
#define LED_DDR     DDRC
#define LED_PORT    PORTC

void led_init(void);
void led_set(uint8_t led, uint8_t enable);

/****************************
 * Analog-Digital-Converter *
 ****************************/
uint16_t adc_read(uint8_t channel);

/****************************
 * Buttons                  *
 ****************************/
uint8_t button_read(void);

/****************************
 * Speaker                  *
 ****************************/
#define SPEAKER_DDR     DDRD
#define SPEAKER_PORT    PORTD
#define SPEAKER_PIN     7

void speaker_init(void);

/*
 * f in Hz, dur in ms
 */
void speaker_sound(uint16_t f, uint16_t dur);

/*
This method plays nokia tune notes.

duration @ 100 bpm:
full note   == 2400 ms
half        == 1200
quarter     == 600
8th         == 300
16th        == 150
32th        == 75

duration @ 200 bpm:
full note   == 1200 ms
half        == 600
quarter     == 300
8th         == 150
16th        == 75
32th        == 37.5

        C1 - H1     C2 - H2     C3 - H3     C4 - F4
        f (Hz=1/s)  f (Hz=1/s)  f (Hz=1/s)  f (Hz=1/s)
C       261,63      523,25      1046,50     2093,00
Cis/Des 277,18      554,37      1108,73     2217,46
D       293,66      587,33      1174,66     2349,32
Dis/Es  311,13      622,25      1244,51     2489,02
E       329,63      659,26      1318,51     2637,02
F       349,23      698,46      1396,91     2793,83
Fis/Ges 369,99      739,99      1479,98
G       392,00      783,99      1567,98
Gis/As  415,30      830,61      1661,21
A       440,00      880,00      1760,00
Ais/B   466,16      932,33      1864,66
H       493,88      987,77      1975,53
*/
void speaker_tune(uint16_t bpm, uint16_t duration, char note, uint16_t octave);

/*
 * ==> Melody:
 * 22 Indiana Jones v2
 * const char* song_str_107 = "4e1 16f1 8g1 2c2 4d1 16e1 1f1 4g1 16a1 8b1
 * 2f2 4a1 16b1 4c2 4d2 4e2 4e1 16f1 8g1 1c2 4d2 16e2 2f2 4g1 16g1 4e2 4d2
 * 16g1 4e2 4d2 16g1 4f2 4e2 16d2 2c2";
 * const unsigned char song_bpm_107 = 125;
 */
void speaker_tune_indy(void);

/*
 * 21 Imperial
 * const char* song_str_105 = "4a1 4a1 4a1 4f1 16c2 4a1 4f1 16c2 2a1 4e2
 * 4e2 4e2 4f2 16c2 4#g1 4f1 16c2 2a1 4a2 4a1 16a1 4a2 4#g2 16g2 16#f2
 * 16f2 4#f2 8#a1 4#d2 4d2 16#c2 16c2 16b1 4c2 8f1 4#g1 4f1 16#g1 4c2 4a1
 * 16c2 2e2 4a2 4a1 16a1 4a2 4#g2 16g2 16#f2 16f2";
 * const unsigned char song_bpm_105 = 112
*/
void speaker_tune_imperial(void);

#endif /* MYAVR_H_ */
