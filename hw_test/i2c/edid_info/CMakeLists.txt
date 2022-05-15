add_executable(edid_info
        edid_info.cpp
)

target_link_libraries(edid_info pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(edid_info)
