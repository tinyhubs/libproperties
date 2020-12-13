cmake_minimum_required(VERSION 3.15)

#   Read version information from local file
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" MY_VERSION)
string(STRIP "${MY_VERSION}" MY_VERSION)

#   Define the project
project(libproperties VERSION ${MY_VERSION} LANGUAGES C)

#   Specify the output dir of the shared library
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

#   We need -fsigned-char for ARM compiler
set(CMAKE_C_FLAGS "-fsigned-char ${CMAKE_C_FLAGS}")

#   Define the target
add_library(properties SHARED buf.c properties.c)
set_target_properties(properties PROPERTIES VERSION     ${CMAKE_PROJECT_VERSION})
set_target_properties(properties PROPERTIES SOVERSION   ${CMAKE_PROJECT_VERSION_MAJOR} )
target_include_directories(properties PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
