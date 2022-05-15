add_executable(pwm_audio
        pwm_audio.c
)

# pull in common dependencies
target_link_libraries(pwm_audio pico_stdlib pico_audio_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(pwm_audio)
