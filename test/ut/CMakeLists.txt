enable_testing()

setup_target_for_coverage_lcov(
    NAME coverage
    EXECUTABLE ctest
    EXCLUDE ${CMAKE_SOURCE_DIR}/test/*
)

macro(add_unit_test unitestname)
    add_executable(${ARGV0} ${ARGN})
    target_include_directories(${ARGV0} PRIVATE ../mock)
    target_compile_definitions(${ARGV0} PRIVATE -DUTEST)
    target_compile_options(${ARGV0} PRIVATE --coverage)
    target_link_libraries(${ARGV0} PRIVATE cmocka-static)
    target_link_options(${ARGV0} PRIVATE --coverage)
    add_test(NAME ${ARGV0} COMMAND $<TARGET_FILE:${ARGV0}>)
    append_coverage_compiler_flags_to_target(${ARGV0})
endmacro()

add_unit_test(ut_user_led
    ../../src/user_led.c
    test_user_led.c
)

add_unit_test(ut_delay
    ../../src/delay.c
    test_delay.c
)

add_unit_test(ut_main
    ../../src/main.c
    ../mock/mock_user_led.c
    ../mock/mock_delay.c
    test_main.c
)
