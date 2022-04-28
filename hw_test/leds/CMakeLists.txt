add_executable(leds
        leds.c
)

target_link_libraries(leds pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(leds)
