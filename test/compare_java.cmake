# run java -jar libproperties_test.jar <input_jar>
execute_process(
    COMMAND "${Java_JAVA_EXECUTABLE}" -jar "${LIBPROPERTIES_JAR}" "${PROPERTIES}"
    OUTPUT_VARIABLE java_stdout
    RESULT_VARIABLE java_result
)

if(java_result)
    message(FATAL_ERROR "Running '${Java_JAVA_EXECUTABLE} -jar ${LIBPROPRTIES_JAR} ${PROPERTIES} failed with ${java_result}")
endif()

# run libproperties_test <input_jar>
execute_process(
    COMMAND "${LIBPROPERTIES_TEST}" "${PROPERTIES}"
    OUTPUT_VARIABLE libproperties_stdout
    RESULT_VARIABLE libproperties_result
)

if(libproperties_result)
    message(FATAL_ERROR "Running '${LIBPROPERTIES_TEST} ${PROPERTIES}' failed with ${libproperties_result}")
endif()

# compare their output

if(java_stdout STREQUAL libproperties_stdout)
    message(STATUS "java and libproperties output match!")
else()
    message(FATAL_ERROR "java and libproperties produced different outputs!")
endif()
