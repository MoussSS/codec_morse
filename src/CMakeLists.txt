set(BUILD_TARGET firmware)

add_executable(${BUILD_TARGET})

target_sources(${BUILD_TARGET} PRIVATE
    main.c
    morse_encoder.c
    morse_decoder.c
    util.c
)

target_compile_options(${BUILD_TARGET} PRIVATE
    -Wall
    -Wextra
    -Werror
    -mcpu=${CPU}
    -mfpu=${FPU}
    -mfloat-abi=${FLOAT_ABI}
)

target_link_options(${BUILD_TARGET} PRIVATE
    -T${LINKER_FILE}
    -mcpu=${CPU}
    -mfpu=${FPU}
    -mfloat-abi=${FLOAT_ABI}
    --specs=nosys.specs
    --specs=nano.specs
    -Wl,-Map=output.map,--cref
    -Wl,--print-memory-usage
)
