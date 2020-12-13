cmake_minimum_required(VERSION 3.4.3)
project(libproperties C)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_C_FLAGS "-fsigned-char ${CMAKE_C_FLAGS}")

add_library(libproperties buf.c properties.c)

target_include_directories(libproperties PRIVATE .)
