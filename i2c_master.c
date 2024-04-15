#include "i2c_master.h"
#include <avr/io.h>
#include <util/twi.h>  // TWI definitions

#define F_SCL 100000UL  // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)


void i2c_init(void) {
    TWBR = (uint8_t)TWBR_val;
    TWSR = 0x00;  // Set prescaler
}

void i2c_start(uint8_t address) {
    // Send START condition
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);

    // Wait for TWINT Flag set
    while (!(TWCR & (1 << TWINT)));

    // Send device address
    TWDR = address;
    TWCR = (1 << TWEN) | (1 << TWINT);

    // Wait for TWINT Flag set
    while (!(TWCR & (1 << TWINT)));
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWEN) | (1 << TWINT);

    // Wait for TWINT Flag set
    while (!(TWCR & (1 << TWINT)));
}

// Updated to use uint8_t instead of bool for the 'ack' parameter
uint8_t i2c_read(uint8_t ack) {
    // Convert ack to a 0 or 1 value for the TWCR register
    if (ack) {
        return i2c_read_ack();
    } else {
        return i2c_read_nack();
    }
}

uint8_t i2c_read_ack(void) {
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t i2c_read_nack(void) {
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void i2c_stop(void) {
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}