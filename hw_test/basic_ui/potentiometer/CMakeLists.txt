add_executable(potentiometer
        potentiometer.c
)

# pull in common dependencies and additional pwm hardware support
target_link_libraries(potentiometer pico_stdlib hardware_pwm hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(potentiometer)
