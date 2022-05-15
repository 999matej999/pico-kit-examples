add_executable(scan_i2c_bus
        scan_i2c_bus.c
)

# pull in common dependencies
target_link_libraries(scan_i2c_bus pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(scan_i2c_bus)
