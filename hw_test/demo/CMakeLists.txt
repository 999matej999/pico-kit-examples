add_executable(demo
        demo.cpp
        SPI.cpp
        SPI.h
        MCP23S018.cpp
        MCP23S018.h
        ssd1306.c
        ssd1306.h
        font.h
)

pico_generate_pio_header(demo ${CMAKE_CURRENT_LIST_DIR}/quadrature_encoder.pio)

target_link_libraries(demo pico_stdlib hardware_i2c hardware_spi hardware_pwm hardware_adc pico_multicore hardware_pio)

# create map/bin/hex file etc.
pico_add_extra_outputs(demo)
