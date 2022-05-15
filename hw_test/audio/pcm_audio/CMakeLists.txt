add_executable(pcm_audio
        pcm_audio.c
)

target_compile_definitions(pcm_audio PRIVATE
        PICO_AUDIO_I2S_CLOCK_PIN_BASE=8
        PICO_AUDIO_I2S_DATA_PIN=10

        AUDIO_FREQ_MAX=48000

        # ours are zero based, so say so
        PICO_USBDEV_USE_ZERO_BASED_INTERFACES=1

        # need large descriptor
        PICO_USBDEV_MAX_DESCRIPTOR_SIZE=256


        PICO_USBDEV_ISOCHRONOUS_BUFFER_STRIDE_TYPE=1
        PICO_USBDEV_ENABLE_DEBUG_TRACE

        PICO_AUDIO_I2S_MONO_OUTPUT=0
        PICO_AUDIO_I2S_MONO_INPUT=0
)


# pull in common dependencies
target_link_libraries(pcm_audio pico_stdlib usb_device pico_audio_i2s pico_multicore)

# create map/bin/hex file etc.
pico_add_extra_outputs(pcm_audio)
