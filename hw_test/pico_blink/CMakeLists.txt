add_executable(pico_blink
        pico_blink.c
)

# pull in common dependencies
target_link_libraries(pico_blink pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico_blink)
