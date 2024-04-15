//amgTest.c
#include <avr/io.h>
#include <util/delay.h>
#include "amg.h"

Adafruit_AMG88xx amg;
float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

// Define pin numbers for LEDs
#define LED_GREEN PIND2
#define LED_YELLOW PIND3
#define LED_RED PIND4


// Function prototypes
void init_sensor(void);
void init_leds(void);
void read_pixels_and_control_leds(void);

int main(void) {
    // Sensor and LED initialization
    init_sensor();
    init_leds();

    while (1) {
        read_pixels_and_control_leds();
        _delay_ms(1000); // delay a second
    }

    return 0;
}

void init_sensor(void) {
    bool status = amg88xx_begin(&amg, AMG88xx_ADDRESS);
    if (!status) {
        // If sensor not found, halt program
        while (1);
    }
}

void init_leds(void) {
    // Set pins 3, 4, and 5 on port D as output
    DDRD |= _BV(DDD2) | _BV(DDD3) | _BV(DDD4);
}

void read_pixels_and_control_leds(void) {
    float maxTemp = 0;
    amg88xx_readPixels(&amg, pixels, AMG88xx_PIXEL_ARRAY_SIZE);

    // Find the highest pixel value
    for (int i = 0; i < AMG88xx_PIXEL_ARRAY_SIZE; i++) {
        if (pixels[i] > maxTemp) {
            maxTemp = pixels[i];
        }
    }

    // Control LEDs based on the max temperature
    // Make sure to clear the LED bits before setting them
    PORTD &= ~(_BV(LED_GREEN) | _BV(LED_YELLOW) | _BV(LED_RED));
    if (maxTemp > 30) {
        // Turn on the RED LED
        PORTD |= _BV(LED_RED);
    } else if (maxTemp > 25) {
        // Turn on the YELLOW LED
        PORTD |= _BV(LED_YELLOW);
    } else {
        // Turn on the GREEN LED
        PORTD |= _BV(LED_GREEN);
    }
}