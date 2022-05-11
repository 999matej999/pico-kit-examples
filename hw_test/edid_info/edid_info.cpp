/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/float.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"

#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3

#define ADDR 0x50

// write
//i2c_write_blocking(i2c1, addr, buf, 2, false);

// read
//i2c_write_blocking(i2c1, addr, &val, 1, true); // true to keep master control of bus
//i2c_read_blocking(i2c1, addr, buffer, 6, false); // false - finished with bus

void decode_edid_info(uint8_t *buffer, size_t len)
{
    if(len != 128) return;
    if(buffer[0] != 0x00) return;
    if(buffer[1] != 0xFF) return;
    if(buffer[2] != 0xFF) return;
    if(buffer[3] != 0xFF) return;
    if(buffer[4] != 0xFF) return;
    if(buffer[5] != 0xFF) return;
    if(buffer[6] != 0xFF) return;
    if(buffer[7] != 0x00) return;

    printf("Header: \n");

    uint8_t word89 = (buffer[8] << 8) | buffer[9];
    uint8_t c0 = 64 + ((word89 >> 10) & 0x1f);
    uint8_t c1 = 64 + ((word89 >> 5) & 0x1f);
    uint8_t c2 = 64 + ((word89 >> 0) & 0x1f);
    printf("  Manufacturer: %c%c%c\n", c0, c1, c2);

    printf("  Product code: %d\n", buffer[10] | (buffer[11] << 8));
    printf("  Serial number: %d\n", buffer[12] | (buffer[13] << 8) | (buffer[14] << 16) | (buffer[15] << 24));
    printf("  Week: %d\n", buffer[16]);
    printf("  Year: %d\n", buffer[16]+1990);
    printf("  Edid version %d, revision %d\n", buffer[18], buffer[19]);

    const char *depth[] = {"undef", "6", "8", "10", "12", "14", "16", "res"};
    const char *interface[] = {"undef", "HDMIa", "HDMIb", "?", "MDDI", "DisplayPort", "?", "?"};
    const char *levels[] = {"+0.7/-.03", "+.714/-.386", "+1.0/-.04", "+.7/0]"};

    if (buffer[20] & 128)
    {
        printf("  Digital input\n");
        printf("  Depth: %s\n", depth[(buffer[20]>>4)&7]);
        printf("  Interface: %s\n", interface[buffer[20]&7]);
    }
    else
    {
        printf("  Analog input\n");
        printf("  Levels: %s\n", levels[(buffer[20]>>5)&3]);
        if (buffer[20] & 16)
        {
            printf("  Blank-to-black setup (pedestal) expected\n");
        }
        if (buffer[20] & 8)
        {
            printf("  Separate sync supported\n");
        }
        if (buffer[20] & 4)
        {
            printf("  Composite sync supported\n");
        }
        if (buffer[20] & 2)
        {
            printf("  Sync on green supported\n");
        }
        if (buffer[20] & 1)
        {
            printf("  VSync pulse must be serrated when composite or sync-on-green is used.\n");
        }
    }

    uint8_t hsize = buffer[21];
    uint8_t vsize = buffer[22];

    if (hsize)
        printf("  Horizontal screen size: %dcm\n", hsize);
    else
        printf("  Portrait aspect ratio: %.3f\n", float(vsize+99)/100);

    if (vsize)
        printf("  Vertical screen size: %dcm\n", vsize);
    else
        printf("  Landscape aspect ratio: %.3f\n", float(hsize+99)/100);

    printf("  Display gamma: %.3f\n", (float(buffer[23])/100 + 1));

    const char *display_type_digital[] = {"RGB 4:4:4", "RGB 4:4:4 + YCrCb 4:4:4", "RGB 4:4:4 + YCrCb 4:2:2", "RGB 4:4:4 + YCrCb 4:4:4 + YCrCb 4:2:2"};
    const char *display_type_analog[] = {"Monochrome or grayscale", "RGB color", "non-RGB color", "undef"};

    if (buffer[24] & 128) printf("  DPMS standby supported\n");
    if (buffer[24] & 64) printf("  DPMS suspend supported\n");
    if (buffer[24] & 32) printf("  DPMS active-off supported\n");

    if (buffer[20] & 128)
        printf("  Display type (digital): %s\n", display_type_digital[(buffer[24]>>3)&3]);
    else
        printf("  Display type (analog): %s\n", display_type_analog[(buffer[24]>>3)&3]);

    if (buffer[24] & 4) printf("  Standard sRGB color space (data not printed)\n");
    if (buffer[24] & 2) printf("  Preferred timing mode in descriptor block 1\n");
    if (buffer[24] & 1) printf("  Continuous timings with GTF or CVT\n");

    float rx = float((buffer[27] << 2) | ((buffer[25]>>6) & 3)) / 1024;
    float ry = float((buffer[28] << 2) | ((buffer[25]>>4) & 3)) / 1024;
    float gx = float((buffer[29] << 2) | ((buffer[25]>>2) & 3)) / 1024;
    float gy = float((buffer[30] << 2) | ((buffer[25]>>0) & 3)) / 1024;
    float bx = float((buffer[31] << 2) | ((buffer[26]>>6) & 3)) / 1024;
    float by = float((buffer[32] << 2) | ((buffer[26]>>4) & 3)) / 1024;
    float wx = float((buffer[33] << 2) | ((buffer[26]>>2) & 3)) / 1024;
    float wy = float((buffer[34] << 2) | ((buffer[26]>>0) & 3)) / 1024;

    printf("Chromaticity coordinates:  r: (%.3f, %.3f), g: (%.3f, %.3f), b: (%.3f, %.3f), w: (%.3f, %.3f)\n", rx, ry, gx, gy, bx, by, wx, wy);
}


int main() {
    stdio_init_all();

    i2c_init(i2c1, 400000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    uint8_t buffer[128] = {};

    printf("Reading from address 0x%02X\n", ADDR);

    uint8_t val = 0x00; // writing 0x00 then reading 128 bytes gives to EDID
    //uint8_t val = 0x80; // writing 0x80 then reading 128 bytes gives to Extended data
    i2c_write_blocking(i2c1, ADDR, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c1, ADDR, buffer, 128, false); // false - finished with bus

    printf("   ");
    for (int j = 0; j < 16; ++j)
    {
        printf(" %X ", j);
    }
    printf("\n");
    for (int i = 0; i < 8; ++i)
    {
        printf("%X0", i);
        for (int j = 0; j < 16; ++j)
        {
            printf(" %02X", buffer[i * 16 + j]);
        }
        printf("\n");
    }

    decode_edid_info(buffer, 128);

    printf("\n");

    val = 0x80; // writing 0x80 then reading 128 bytes gives to Extended data
    i2c_write_blocking(i2c1, ADDR, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c1, ADDR, buffer, 128, false); // false - finished with bus

    printf("   ");
    for (int j = 0; j < 16; ++j)
    {
        printf(" %X ", j);
    }
    printf("\n");
    for (int i = 0; i < 8; ++i)
    {
        printf("%X0", i);
        for (int j = 0; j < 16; ++j)
        {
            printf(" %02X", buffer[i * 16 + j]);
        }
        printf("\n");
    }

    while (1)
    {
        sleep_ms(100);
    }
}
