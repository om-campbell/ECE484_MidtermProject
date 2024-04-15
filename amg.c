//amg.c
#include "amg.h"
#include "i2c_master.h"

void amg88xx_i2c_init(uint8_t i2c_addr) {
    i2c_init();
}

bool amg88xx_begin(Adafruit_AMG88xx *device, uint8_t addr) {
    device->i2c_address = addr;
    
    amg88xx_i2c_init(device->i2c_address);
    
    // Enter normal mode
    amg88xx_write8(device, AMG88xx_PCTL, AMG88xx_NORMAL_MODE);
    
    // Software reset
    amg88xx_write8(device, AMG88xx_RST, AMG88xx_INITIAL_RESET);
    
    // Disable interrupts by default
    //amg88xx_disableInterrupt(device);
    
    // Set to 10 FPS
    amg88xx_write8(device, AMG88xx_FPSC, AMG88xx_FPS_10);

    return true;
}

void amg88xx_write8(Adafruit_AMG88xx *device, uint8_t reg, uint8_t value) {
    i2c_start((device->i2c_address << 1) | TW_WRITE);
    i2c_write(reg);
    i2c_write(value);
    i2c_stop();
}

void amg88xx_read(Adafruit_AMG88xx *device, uint8_t reg, uint8_t *buf, uint8_t num) {
    i2c_start((device->i2c_address << 1) | TW_WRITE);
    i2c_write(reg);
    i2c_stop();

    i2c_start((device->i2c_address << 1) | TW_READ);
    for (uint8_t i = 0; i < num; i++) {
        buf[i] = i < (num - 1) ? i2c_read_ack() : i2c_read_nack();
    }
    i2c_stop();
}

void amg88xx_readPixels(Adafruit_AMG88xx *device, float *buf, uint8_t size) {
    if (size > AMG88xx_PIXEL_ARRAY_SIZE) {
        size = AMG88xx_PIXEL_ARRAY_SIZE; // Ensure we don't read more than the sensor array
    }

    uint16_t recast;
    float converted;
    uint8_t bytesToRead = size << 1; // Each pixel consists of two bytes
    uint8_t rawArray[AMG88xx_PIXEL_ARRAY_SIZE << 1]; // Double size for two bytes per pixel

    // Read the pixel data from the sensor
    amg88xx_read(device, AMG88xx_PIXEL_OFFSET, rawArray, bytesToRead);

    // Convert raw data to temperatures and store in the provided buffer
    for (uint8_t i = 0; i < size; i++) {
        uint8_t pos = i << 1;
        recast = ((uint16_t)rawArray[pos + 1] << 8) | ((uint16_t)rawArray[pos]);

        converted = amg88xx_signedMag12ToFloat(recast) * AMG88xx_PIXEL_TEMP_CONVERSION;
        buf[i] = converted;
    }
}

float amg88xx_signedMag12ToFloat(uint16_t val) {
    uint16_t absVal = (val & 0x7FF);
    return (val & 0x800) ? -(float)absVal : (float)absVal;
}