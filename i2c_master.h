#ifndef I2C_MASTER_H
#define I2C_MASTER_H

#include <stdint.h>

#ifndef TW_WRITE
#define TW_WRITE 0 // Or the appropriate value for your hardware
#endif

#ifndef TW_READ
#define TW_READ 1 // Or the appropriate value for your hardware
#endif

void i2c_init(void);
void i2c_start(uint8_t address);
void i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);  // Read with ACK
uint8_t i2c_read_nack(void); // Read with NACK
void i2c_stop(void);

// We will use uint8_t instead of bool for the 'ack' parameter
uint8_t i2c_read(uint8_t ack);

#endif // I2C_MASTER_H
