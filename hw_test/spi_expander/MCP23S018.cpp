#include "MCP23S018.h"
#include "hardware/gpio.h"

MCP23S018::MCP23S018(SPI &spi_, uint cs_pin_, uint8_t deviceOpcode_) : spi(spi_), cs_pin(cs_pin_), deviceOpcode(deviceOpcode_)
{
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);
    // Make the CS pin available to picotool
    //bi_decl(bi_1pin_with_name(PICO_DEFAULT_SPI_CSN_PIN, "SPI CS"));
}

MCP23S018::~MCP23S018()
{
}

uint8_t MCP23S018::read()
{
    uint8_t msg[2] = {};
    msg[0] = deviceOpcode | 1u;
    msg[1] = static_cast<uint8_t>(REG::GPIO);
    uint8_t value = 0;
    spi.read_registers(cs_pin, msg, 2, &value, 1);
    return value;
}

void MCP23S018::write(uint8_t value)
{
    uint8_t msg[3] = {};
    msg[0] = deviceOpcode;
    msg[1] = static_cast<uint8_t>(REG::OLAT);
    msg[2] = value;
    spi.write_register(cs_pin, msg, 3);
}

void MCP23S018::setDirection(uint8_t direction)
{
    uint8_t msg[3] = {};
    msg[0] = deviceOpcode;
    msg[1] = static_cast<uint8_t>(REG::IODIR);
    msg[2] = direction;
    spi.write_register(cs_pin, msg, 3);
}

// 0 = pull-ip disabled, 1 = pull-up enabled
void MCP23S018::setPullup(uint8_t pullup)
{
    uint8_t msg[3] = {};
    msg[0] = deviceOpcode;
    msg[1] = static_cast<uint8_t>(REG::GPPU);
    msg[2] = pullup;
    spi.write_register(cs_pin, msg, 3);
}
