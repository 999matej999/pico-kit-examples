add_executable(eeprom
        eeprom.cpp
        AT24Cxx.h
        AT24Cxx.cpp
)

# pull in common dependencies
target_link_libraries(eeprom pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(eeprom)
