cmake_minimum_required(VERSION 3.15)

#   Read version information from local file
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/VERSION" MY_VERSION)
string(STRIP "${MY_VERSION}" MY_VERSION)

#   Define the project
project(libproperties VERSION ${MY_VERSION} LANGUAGES C)

#   Define GNU standard installation directories
include(GNUInstallDirs)

#   We need -fsigned-char for ARM compiler
set(CMAKE_C_FLAGS "-fsigned-char ${CMAKE_C_FLAGS}")

#   Define the target
add_library(properties buf.c properties.c)
set_target_properties(properties PROPERTIES
    VERSION     ${PROJECT_VERSION}
    SOVERSION   ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME libproperties
)
target_include_directories(properties PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#   Install the library + header + cmake config file
option(LIBPROPERTIES_INSTALL "Install libproperties" ON)
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
option(LIBPROPERTIES_TEST "Build the libproperties tests" ON)
if(LIBPROPERTIES_TEST)
    find_package(Java REQUIRED COMPONENTS Runtime Development)
    include(UseJava)

    add_jar(libproperties_test_jar test/Main.java
        OUTPUT_NAME libproperties_test
        ENTRY_POINT Main
    )

    add_executable(libproperties_test properties_test.c)
    target_link_libraries(libproperties_test PRIVATE properties)

    enable_testing()

    function(libproproperties_add_test_compare NAME PROPERTIES)
        add_test(NAME ${NAME}
            COMMAND "${CMAKE_COMMAND}"
                "-DJava_JAVA_EXECUTABLE=${Java_JAVA_EXECUTABLE}"
                "-DLIBPROPERTIES_JAR=${CMAKE_CURRENT_BINARY_DIR}/libproperties_test.jar"
                "-DLIBPROPERTIES_TEST=$<TARGET_FILE:libproperties_test>"
                "-DPROPERTIES=${PROPERTIES}"
                -P "${CMAKE_CURRENT_SOURCE_DIR}/test/compare_java.cmake"
        )
        set_tests_properties(${NAME} PROPERTIES TIMEOUT 5)
    endfunction()

    libproproperties_add_test_compare(libproperties_test_1 "${CMAKE_CURRENT_SOURCE_DIR}/test/1.properties")
    libproproperties_add_test_compare(libproperties_test_2 "${CMAKE_CURRENT_SOURCE_DIR}/test/2.properties")
endif()
