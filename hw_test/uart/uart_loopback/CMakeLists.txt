add_executable(uart_loopback
        uart_loopback.c
)

# pull in common dependencies
target_link_libraries(uart_loopback pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(uart_loopback)
