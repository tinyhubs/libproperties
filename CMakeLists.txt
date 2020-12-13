cmake_minimum_required(VERSION 3.15)
project(libproperties C)


file(READ VERSION PROJECT_VERSION)
message("${PROJECT_VERSION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS "-fsigned-char ${CMAKE_C_FLAGS}")


add_library(libproperties SHARED buf.c properties.c)

set_target_properties(libproperties PROPERTIES 
	VERSION		${PROJECT_VERSION}
	SOVERSION	${PROJECT_VERSION} )

target_include_directories(libproperties PRIVATE .)
