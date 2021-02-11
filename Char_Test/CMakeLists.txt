cmake_minimum_required(VERSION 3.12)

# Pull in PICO SDK (must be before project)
include(../pico_sdk_import.cmake)

project(ct C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# Initialize the SDK
pico_sdk_init()

add_executable(ct
        ../ssd1306.cpp main.cpp
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(ct pico_stdlib hardware_i2c hardware_adc)

# create map/bin/hex file etc.
pico_add_extra_outputs(ct)

