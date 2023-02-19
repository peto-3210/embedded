cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(knightrider C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(knightrider
  knightrider.c
)

pico_enable_stdio_usb(knightrider 1)
pico_enable_stdio_uart(knightrider 0)

pico_add_extra_outputs(knightrider)

target_link_libraries(knightrider pico_stdlib)