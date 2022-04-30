#include "MCP23S018.h"

MCP23S018::MCP23S018(SPI &spi_) : spi(spi_)
{
}

MCP23S018::~MCP23S018()
{
}

uint8_t MCP23S018::read()
{
    uint8_t value = 0;
    spi.read_registers(static_cast<uint8_t>(REG::GPIO), &value, 1);
    return value;
}

void MCP23S018::write(uint8_t value)
{
    spi.write_register(static_cast<uint8_t>(REG::OLAT), value);
}

void MCP23S018::setDirection(uint8_t direction)
{
    spi.write_register(static_cast<uint8_t>(REG::IODIR), direction);
}

// 0 = pull-ip disabled, 1 = pull-up enabled
void MCP23S018::setPullup(uint8_t pullup)
{
    spi.write_register(static_cast<uint8_t>(REG::GPPU), pullup);
}
