#!/usr/bin/bash



SELFDIR=$(cd $(dirname $0);pwd)
TESTDIR="${SELFDIR}"



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
    echo    "Generate expect files ..."
    generate_expect_files
    RESULT=$?
    if [ $RESULT -ne 0 ]; then
        echo    "Generate expect files failed"
        return  2
    fi
    echo    "Generate expect files success"

    return  0
}



main    "$@"
exit    "$?"
