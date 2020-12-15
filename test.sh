#!/usr/bin/bash



SELFDIR=$(cd $(dirname $0);pwd)
TESTDIR="${SELFDIR}/test"



function    compile_java_tools()
{
    rm -rf  ${TESTDIR}/*.class
    cd      "${TESTDIR}" && javac "${TESTDIR}/Main.java"
    RESULT=$?
    if [ ${RESULT} -ne 0 ] || [ ! -f ${TESTDIR}/Main.class ]; then
        echo    "Compile the java test tool failed"
        return  1
    fi 

    return  0
}



function    compile_c_tools()
{
    #premake5 --file=libproperties_test.premake clean
    #if [ "x${OS}" == "xWindows_NT" ]; then
    #    premake5 --file=libproperties_test.premake vs2010
    #else
    #    premake5 --file=libproperties_test.premake gmake
    #fi
    #RESULT=$?
    #if [ ${RESULT} -ne 0 ]; then
    #    echo    "Generate project file failed"
    #    return  1
    #fi


    #if [ "x${OS}" == "xWindows_NT" ]; then
        
    #else
    #fi

    return  0
}




function    generate_expect_files()
{
    local files=$(cd ${TESTDIR};ls | grep -E '[0-9]+\.properties$')
    for file in ${files}; do
        rm -rf  "${TESTDIR}/${file}.j.expect"
        cd      "${TESTDIR}" && java -cp "./" Main "${TESTDIR}/${file}" > "${TESTDIR}/${file}.j.expect"
        RESULT=$?
        if [ ${RESULT} -ne 0 ] || [ ! -f "${file}.j.expect" ]; then
            echo    "Create expect file failed: ${file}"
            return  1
        fi 
    done

    return  0
}




function    run_testcases()
{
    local exe_tool=${TESTDIR}/libproperties_test
    if [ "x${OS}" == "xWindows_NT" ]; then
        exe_tool=${TESTDIR}/libproperties_test.exe
    fi

    if [ ! -f "${exe_tool}" ]; then
        echo    "The C test tool is not exist"
        return  3
    fi


    local fail_count=0
    local files=$(cd "${TESTDIR}";ls | grep -E '[0-9]+\.properties$')
    for file in ${files}; do
        rm -rf  "${TESTDIR}/${file}.c.expect"
        cd      "${TESTDIR}" && ${exe_tool} "${TESTDIR}/${file}" > "${TESTDIR}/${file}.c.expect"
        RESULT=$?
        if [ ${RESULT} -ne 0 ] || [ ! -f "${file}.c.expect" ]; then
            echo    "Parse file failed: ${TESTDIR}/${file}"
            return  4
        fi

        dos2unix    "${TESTDIR}/${file}.c.expect"   >&  /dev/null
        dos2unix    "${TESTDIR}/${file}.j.expect"   >&  /dev/null

        diff -a -q -y  "${TESTDIR}/${file}.c.expect"   "${TESTDIR}/${file}.j.expect"
        RESULT=$?
        if [ ${RESULT} -ne 0 ]; then
            echo    "[${file}]    fail"
            fail_count=`expr ${fail_count} + 1`
        else
            echo    "[${file}]    ok"
        fi
    done


    if [ ${fail_count} -ne 0 ]; then
        echo    "Test failed"
        return  1
    fi

    return  0
}




function main()
{
    echo    "----"
    echo    "Compile java test tool ..."
    compile_java_tools
    RESULT=$?
    if [ $RESULT -ne 0 ]; then
        echo    "Compile java test tool failed"
        return  1
    fi
    echo    "Compile java test tool success"


    echo    "----"
    echo    "Compile c test tool ..."
    compile_c_tools
    RESULT=$?
    if [ $RESULT -ne 0 ]; then
        echo    "Compile c test tool failed"
        return  2
    fi
    echo    "Compile c test tool success"


    echo    "----"
    echo    "Generate expect files ..."
    generate_expect_files
    RESULT=$?
    if [ $RESULT -ne 0 ]; then
        echo    "Generate expect files failed"
        return  2
    fi
    echo    "Generate expect files success"


    echo    "----"
    echo    "Run testcases ..."
    echo    ""
    run_testcases
    RESULT=$?
    echo    ""
    if [ $RESULT -ne 0 ]; then
        echo    "--------------------------------"
        echo    "Run testcases failed"
        return  2
    else
        echo    "--------------------------------"
        echo    "Run testcases success"
        return  0
    fi
}



main    "$@"
exit    "$?"
