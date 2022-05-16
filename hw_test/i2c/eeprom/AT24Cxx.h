#ifndef __AT24CXX_H__
#define __AT24CXX_H__

#define AT24CXX_ADDR 0x51 // device address

#include <stdint.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

void AT24Cxx_write(uint8_t page, uint8_t offset, uint8_t *data, size_t size);
void AT24Cxx_read(uint8_t page, uint8_t offset, uint8_t *data, size_t size);

#endif // __AT24CXX_H__ //
