# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(SMART_IO)

target_sources(app PRIVATE src/main.c src/UART/UART.c src/sensors/adc.c src/sensors/leds.c src/sensors/buttons.c src/sensors/rtdb.c)
target_include_directories(app PRIVATE src/UART src/sensors)
