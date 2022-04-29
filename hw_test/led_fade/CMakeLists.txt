add_executable(pwm_led_fade
        pwm_led_fade.c
        )

# pull in common dependencies and additional pwm hardware support
target_link_libraries(pwm_led_fade pico_stdlib hardware_pwm hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(pwm_led_fade)
