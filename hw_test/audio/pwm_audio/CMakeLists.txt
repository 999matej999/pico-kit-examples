add_executable(pwm_audio
        pwm_audio.c
)

# pull in common dependencies
target_link_libraries(pwm_audio pico_stdlib pico_audio_pwm)

target_compile_definitions(pwm_audio PRIVATE
            #define for our example code
            PICO_AUDIO_PWM_L_PIN=8
            PICO_AUDIO_PWM_R_PIN=9
            )

# create map/bin/hex file etc.
pico_add_extra_outputs(pwm_audio)
