#ifndef __MCP23S018_H__
#define __MCP23S018_H__

#include <stdint.h>
#include "SPI.h"

class MCP23S018
{

private:
    enum class REG : uint8_t
    {
        IODIR       = 0x00,
        IPOL        = 0x01,
        GPINTEN     = 0x02,
        DEFVAL      = 0x03,
        INTCON      = 0x04,
        IOCON       = 0x05,
        GPPU        = 0x06,
        INTF        = 0x07,
        INTCAP      = 0x08,
        GPIO        = 0x09,
        OLAT        = 0x0A
    };

    SPI &spi;
    const uint cs_pin;
    const uint8_t deviceOpcode;

public:
    MCP23S018(SPI &spi_, uint cs_pin_, uint8_t deviceOpcode_);
    ~MCP23S018();

    void setDirection(uint8_t direction);
    void setPullup(uint8_t pullup);
    uint8_t read();
    void write(uint8_t value);

};

#endif /* __MCP23S018_H__ */
