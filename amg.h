//amg.h
#ifndef LIB_ADAFRUIT_AMG88XX_H
#define LIB_ADAFRUIT_AMG88XX_H

#include <stdint.h>
#include <stdbool.h>
#include "i2c_master.h" // Include I2C functions

// I2C Address for the sensor
#define AMG88xx_ADDRESS (0x69)

// Register definitions
#define AMG88xx_PCTL                   0x00
#define AMG88xx_RST                    0x01
#define AMG88xx_FPSC                   0x02
#define AMG88xx_INTC                   0x03
#define AMG88xx_STAT                   0x04
#define AMG88xx_SCLR                   0x05

// Normal operation mode value, adjust based on the sensor's datasheet
#define AMG88xx_NORMAL_MODE        0x00

// Reset modes - these should be the actual values specified by the datasheet
#define AMG88xx_INITIAL_RESET      0x3F
#define AMG88xx_FLAG_RESET         0x30

// Frame rate control - these should be the actual values specified by the datasheet
#define AMG88xx_FPS_10             0x00
#define AMG88xx_FPS_1              0x01

// 0x06 reserved
#define AMG88xx_AVE                    0x07
#define AMG88xx_INTHL                  0x08
#define AMG88xx_INTHH                  0x09
#define AMG88xx_INTLL                  0x0A
#define AMG88xx_INTLH                  0x0B
#define AMG88xx_IHYSL                  0x0C
#define AMG88xx_IHYSH                  0x0D
#define AMG88xx_TTHL                   0x0E
#define AMG88xx_TTHH                   0x0F
#define AMG88xx_INT_OFFSET             0x010
#define AMG88xx_PIXEL_OFFSET           0x80

#define AMG88xx_PIXEL_ARRAY_SIZE       64
#define AMG88xx_PIXEL_TEMP_CONVERSION  0.25
#define AMG88xx_THERMISTOR_CONVERSION  0.0625

// Struct to hold the device context
typedef struct {
    uint8_t i2c_address;
} Adafruit_AMG88xx;

// Function prototypes
bool amg88xx_begin(Adafruit_AMG88xx *device, uint8_t addr);
void amg88xx_readPixels(Adafruit_AMG88xx *device, float *buf, uint8_t size);
float amg88xx_readThermistor(Adafruit_AMG88xx *device);
void amg88xx_write8(Adafruit_AMG88xx *device, uint8_t reg, uint8_t value);
void amg88xx_setMovingAverageMode(Adafruit_AMG88xx *device, bool mode);
void amg88xx_enableInterrupt(Adafruit_AMG88xx *device);
//void amg88xx_disableInterrupt(Adafruit_AMG88xx *device);
void amg88xx_setInterruptMode(Adafruit_AMG88xx *device, uint8_t mode);
void amg88xx_getInterrupt(Adafruit_AMG88xx *device, uint8_t *buf, uint8_t size);
void amg88xx_clearInterrupt(Adafruit_AMG88xx *device);
void amg88xx_setInterruptLevels(Adafruit_AMG88xx *device, float high, float low, float hysteresis);
float amg88xx_signedMag12ToFloat(uint16_t val);
#endif
