add_executable(oled
	example.c ssd1306.c
)

target_include_directories(oled
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../
)

target_link_libraries(oled pico_stdlib hardware_i2c)

pico_enable_stdio_usb(oled 1) 
pico_enable_stdio_uart(oled 0) 

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(oled)

