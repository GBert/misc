cmake_minimum_required(VERSION 2.8.8)
project(si4010prog C)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -Wall)

# Build Options
option(WITH_C2_BUS_FT232 "Enable FTDI FT232 based C2 Bus interface" OFF)
option(WITH_C2_BUS_FX2 "Enable Cypress FX2 based C2 Bus interface" ON)
option(WITH_C2_BUS_C2DRV "Enable Linux GPIO based C2 Bus interface" ON)

configure_file(config.h.in "${PROJECT_BINARY_DIR}/config.h")
include_directories("${PROJECT_BINARY_DIR}")

include(GitGetVersion)

#TODO: detect libusb
#TODO: detect libftdi

add_subdirectory(src)
