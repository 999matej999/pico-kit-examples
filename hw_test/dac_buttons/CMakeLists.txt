add_executable(dac_buttons
        dac_buttons.c
)

target_link_libraries(dac_buttons pico_stdlib hardware_adc)

# enable usb output, disable uart output
# pico_enable_stdio_usb(dac_buttons 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(dac_buttons)
