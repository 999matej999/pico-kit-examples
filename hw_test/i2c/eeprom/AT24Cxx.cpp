#include "AT24Cxx.h"

#define PAGE_SIZE 32

AT24Cxx::AT24Cxx(uint8_t addr_): addr(addr_)
{
}

AT24Cxx::~AT24Cxx()
{
}

void AT24Cxx::write(uint8_t page, uint8_t offset, uint8_t *data, size_t size)
{
    uint16_t addr = (page << 5) | offset;

    uint8_t *val = new uint8_t[2 + size];

    val[0] = (addr & 0xFF00) >> 8;
    val[1] = addr & 0x00FF;

    for(size_t i = 0; i < size; ++i) val[2 + i] = data[i];

    i2c_write_blocking(i2c1, this->addr, val, 2 + size, false); // false - finished with bus
    
    delete[] val;
}

void AT24Cxx::read(uint8_t page, uint8_t offset, uint8_t *data, size_t size)
{
    uint16_t addr = (page << 5) | offset;
    
    uint8_t val[2] = {};
    val[0] = (addr & 0xFF00) >> 8;
    val[1] = addr & 0x00FF;

    i2c_write_blocking(i2c1, this->addr, val, 2, true); // true to keep master control of bus
    i2c_read_blocking(i2c1, this->addr, data, size, false);  // False - finished with bus
}
