#include "gspi.h"

#include <avr/io.h>
#include <util/delay.h>

#include <inttypes.h>

// CS PB1
// MOSI PB3
// MISO PB4
// SCLK PB5
int main(void)
{
    uint16_t data = 0;

    // Init PORTB
    PORTB = 0xff;
    DDRB = 0;                   /* PB4 input */
    DDRB |= 0x2a;               /* PB1, PB3, PB5 output */
    DDRB |= 1 << 6;             /* PB6 output */
    DDRB |= 1 << 7;             /* PB7 output */
    /* Init PORTC */
    PORTC = 0xff;
    DDRC = 0;                   /* PC3 input */
    /* Init PORTD */
    PORTD = 0xff;
    DDRD = 0;                   /* PD5 input */
    DDRD |= 1 << 6;             /* PD6 output */
    DDRD |= 1 << 7;             /* PD7 output */

    gspi_initialize();
 
    for (;;) {    
        if (!(PINC & (1 << 3))) {  /* PC3 */
            PORTB &= ~(1 << 6);
            _delay_ms(1);
            PORTB |= 1 << 6;
        }

        if (PIND & (1 << 5)) {
            PORTB |= 1 << 7;
            gspi_read(0x22, &data);
            if (data == 0) {
                PORTD &= ~(1 << 6);
                PORTD |= 1 << 7;
            } else if (data == 1) {
                PORTD |= 1 << 6;
                PORTD &= ~(1 << 7);
            } else if (data == 2) {
                PORTD |= (1 << 6) | (1 << 7);
            }
        } else {
            PORTB &= ~(1 << 7);
        }
    }
}
