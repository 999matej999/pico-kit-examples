#ifndef __AT24CXX_H__
#define __AT24CXX_H__

#include <stdint.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

class AT24Cxx
{

private:
    i2c_inst_t *i2c;
    const uint8_t addr;

public:
    AT24Cxx(i2c_inst_t *i2c_, uint8_t addr_);
    ~AT24Cxx();

    void write(uint8_t page, uint8_t offset, uint8_t *data, size_t size);
    void read(uint8_t page, uint8_t offset, uint8_t *data, size_t size);

};

#endif // __AT24CXX_H__ //
