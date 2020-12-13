cmake_minimum_required(VERSION 3.15)

file(READ VERSION MY_VERSION)
string(STRIP ${MY_VERSION} MY_VERSION)
message("[${MY_VERSION}]")

project(libproperties VERSION ${MY_VERSION} LANGUAGES C)

message("${CMAKE_PROJECT_VERSION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS "-fsigned-char ${CMAKE_C_FLAGS}")


add_library(properties SHARED buf.c properties.c)

set_target_properties(properties PROPERTIES VERSION     ${CMAKE_PROJECT_VERSION})
set_target_properties(properties PROPERTIES SOVERSION   ${CMAKE_PROJECT_VERSION_MAJOR} )

target_include_directories(properties PRIVATE .)
