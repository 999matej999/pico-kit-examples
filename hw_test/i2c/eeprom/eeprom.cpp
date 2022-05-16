#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "AT24Cxx.h"

#define I2C_SDA_PIN 2
#define I2C_SCL_PIN 3

int main()
{
    // Enable UART so we can print status output
    stdio_init_all();
    
    // This example will use I2C1 on the SDA and SCL pins
    i2c_init(i2c1, 10 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    printf("EEPROM EXAMPLE\n");

    uint8_t send_buf[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    uint8_t recv_buf[10] = {};

    printf("Send buffer:");
    for (size_t i = 0; i < 10; ++i)
    {
        printf("\t%d", send_buf[i]);
    }
    printf("\n");

    AT24Cxx_write(0, 0, send_buf, 10);

    sleep_ms(200);

    AT24Cxx_read(0, 0, recv_buf, 10);

    printf("Recv buffer:");
    for (size_t i = 0; i < 10; ++i)
    {
        printf("\t%d", recv_buf[i]);
    }
    printf("\n");

    return 0;
}
