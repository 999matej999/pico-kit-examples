add_executable(pio_quadrature_encoder
        quadrature_encoder.c
)

pico_generate_pio_header(pio_quadrature_encoder ${CMAKE_CURRENT_LIST_DIR}/quadrature_encoder.pio)

target_link_libraries(pio_quadrature_encoder PRIVATE pico_stdlib pico_multicore hardware_pio)

#pico_enable_stdio_uart(pio_quadrature_encoder 1)

pico_add_extra_outputs(pio_quadrature_encoder)
