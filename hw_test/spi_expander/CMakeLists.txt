add_executable(spi_expander
    spi_expander.cpp
    SPI.cpp
    SPI.h
    MCP23S018.cpp
    MCP23S018.h
)

# pull in common dependencies
target_link_libraries(spi_expander pico_stdlib hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(spi_expander)
