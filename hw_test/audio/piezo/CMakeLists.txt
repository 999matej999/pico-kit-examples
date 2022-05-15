add_executable(piezo
        piezo.c
)

target_link_libraries(piezo pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(piezo)
