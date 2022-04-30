#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>

#define SPI_RX_PIN  4
#define SPI_SCK_PIN 6
#define SPI_TX_PIN  7
#define SPI_CSN_PIN 26

class SPI
{

private:
    const uint8_t deviceOpcode = 0b01000000;

public:
    SPI(/* args */);
    ~SPI();
    void write_register(uint8_t reg, uint8_t data);
    void read_registers(uint8_t reg, uint8_t *buf, uint16_t len);


};

#endif /* __SPI_H__ */
