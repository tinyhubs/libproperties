cmake_minimum_required(VERSION 3.15)


#   Defined all of the options
option(BUILD_SHARED_LIBS     "Build the shared lib" ON)
option(LIBPROPERTIES_INSTALL "Install libproperties" ON)
option(LIBPROPERTIES_TEST    "Build the libproperties tests" ON)


#   Read version information from local file
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" LIBPROPERTIES_VERSION)
string(STRIP "${LIBPROPERTIES_VERSION}" LIBPROPERTIES_VERSION)

#   Define the project with version
project(libproperties VERSION ${LIBPROPERTIES_VERSION} LANGUAGES C)

#   Define GNU standard installation directories
include(GNUInstallDirs)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

#   Define the target
add_library(properties buf.c properties.c)
set_target_properties(properties PROPERTIES
    VERSION             ${PROJECT_VERSION}
    SOVERSION           ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME         libproperties
    DEFINE_SYMBOL       LIBPROPERTIES_EXPORT
    C_VISIBILITY_PRESET hidden
)
target_include_directories(properties PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#   Decide build as shared lib or not
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(properties PUBLIC LIBPROPERTIES_STATIC)
endif()

# FACK CMAKE: What is this shit like syntax!!
target_compile_options(properties PRIVATE
    $<$<NOT:$<STREQUAL:$<C_COMPILER_ID>,MSVC>>:
        #   We need -fsigned-char for ARM compiler
        -fsigned-char
    >)

#   Install the library + header + cmake config file
if(LIBPROPERTIES_INSTALL)
    install(TARGETS properties EXPORT properties
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )
    install(FILES properties.h
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
    install(EXPORT properties NAMESPACE "libproperties::"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECTNAME}"
        FILE "libproperties-config.cmake"
    )
endif()

#   Build + add the test target
if(LIBPROPERTIES_TEST)
    #   Add executable target for test
    add_executable(libproperties_test properties_test.c)
    target_link_libraries(libproperties_test PRIVATE properties)

    #   Enable testing
    enable_testing()

    #   Defined a function to add new test case
    function(libproproperties_add_test casefile)
        get_filename_component(casename "${casefile}" NAME)
        add_test("${casename}" "libproperties_test" "${casefile}" "${casename}.c.output" "${casefile}.j.expect")
        set_tests_properties(${casename} PROPERTIES TIMEOUT 5)
    endfunction()

    #   Scan file under test and add to ctest
    file(GLOB ALL_TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/test/*.properties)
    foreach(casename ${ALL_TEST_FILES})
        libproproperties_add_test("${casename}")
    endforeach()
endif()
