cmake_minimum_required(VERSION 3.20)

if(CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL Release)
    set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-none-eabi.cmake)
elseif(CMAKE_BUILD_TYPE STREQUAL Test)
    set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-native.cmake)
else()
    message(FATAL_ERROR " Specify CMAKE_BUILD_TYPE as Debug, Release or Test")
endif()

project(STM32-FROM-SCRATCH)

enable_language(C ASM)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(LINKER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/config/nucleo-f446re.ld)
set(OPENOCD_CFG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/config/nucleo-f446re.cfg)

if(CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL Release)
    add_subdirectory(src)
elseif(CMAKE_BUILD_TYPE STREQUAL Test)
    include(CTest)
    include(cmake/cmocka.cmake)
    include(cmake/code-coverage.cmake)
    add_subdirectory(test/ut)
    add_subdirectory(test/functional)
endif()