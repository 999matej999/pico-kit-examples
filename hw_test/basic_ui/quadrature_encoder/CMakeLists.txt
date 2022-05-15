add_executable(quadrature_encoder
        quadrature_encoder.c
)

pico_generate_pio_header(quadrature_encoder ${CMAKE_CURRENT_LIST_DIR}/quadrature_encoder.pio)

target_link_libraries(quadrature_encoder PRIVATE pico_stdlib pico_multicore hardware_pio)

#pico_enable_stdio_usb(quadrature_encoder 1)

pico_add_extra_outputs(quadrature_encoder)
