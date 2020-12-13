#!/usr/bin/bash

SELFDIR=$(realpath $(dirname "$0"))
PROJECT_NAME=$(cat    "${SELFDIR}/NAME")
PROJECT_VERSION=$(cat "${SELFDIR}/VERSION")
PROJECT_HARDWARE=$(uname -m)
PROJECT_SYSTEM=$(uname -s)

function do_help()
{
    echo    "Usage:"
    echo    "   build.sh [debug|release]"
    echo    "   build.sh [compile] [debug|release]"
    echo    "   build.sh package"
    echo    "   build.sh test"
    echo    "   build.sh format"
    echo    "   build.sh encode"
    echo    "   build.sh clean"
    echo    "   build.sh help|-h|--help"
    return  0
}

#   $1  compile
#   $2  mode(debug or release)
function do_compile()
{
    rm -rf "${SELFDIR}/build"
    
    mkdir -p "${SELFDIR}/build"
    cd "${SELFDIR}/build" && \
    cmake ../             && \
    make  clean           && \
    make
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        echo   "Error: Compile project '${PROJECT_NAME}' with mode '$2' failed(${RESULT})"
        return 1
    fi
    
    echo   "Compile project '${PROJECT_NAME}' with mode '$2' success"
    return 0
}

function do_package()
{
    rm -rf      "${SELFDIR}"/*.tar.gz
    rm -rf      "${SELFDIR}"/*.tar
    rm -rf      "${SELFDIR}/pack"

    mkdir   -p  "${SELFDIR}/pack/include"
    mkdir   -p  "${SELFDIR}/pack/lib"
    mkdir   -p  "${SELFDIR}/pack/licenses"

    local package_name="${PROJECT_NAME}-${PROJECT_SYSTEM}-${PROJECT_HARDWARE}-${PROJECT_VERSION}"

    cd          "${SELFDIR}/pack"                                       &&  \
    cp -Rf      "${SELFDIR}/lib"/*          "${SELFDIR}/pack/lib/"      &&  \
    cp -rf      "${SELFDIR}/properties.h"   "${SELFDIR}/pack/include/"  &&  \
    cp -rf      "${SELFDIR}/LICENSE"        "${SELFDIR}/pack/licenses/" &&  \
    tar -cvzf   "${package_name}.tar.gz"    *                           &&  \
    mv          "${package_name}.tar.gz"    "${SELFDIR}"
    RESULT=$?
    if [[ ${RESULT} -ne 0 ]]; then
        echo    "Error: Create package failed(${RESULT})"
        return  2
    fi

    #rm -rf      "${SELFDIR}/pack"

    return  0
}

function do_test()
{
    echo    "TODO do_test"
}

function do_format()
{
    echo    "TODO do_format"
}

function do_encode()
{
    echo    "TODO do_encode"
}

function do_clean()
{
    echo    "TODO do_clean"
}

function main()
{
    if   [[ "$1" == "" ]]; then
        do_compile  "compile"   "debug"
        return      "$?"
    elif [[ "$1" == "debug" ]] || [[ "$1" == "release" ]]; then
        do_compile  "compile"   "$@"
        return      "$?"
    elif [[ "$1" == "compile" ]]; then
        do_compile  "$@"
        return      "$?"
    elif [[ "$1" == "test" ]]; then
        do_test     "$@"
        return      "$?"
    elif [[ "$1" == "package" ]]; then
        do_package  "$@"
        return      "$?"
    elif [[ "$1" == "help" ]] || [[ "$1" == "-h" ]] || [[ "$1" == "--help" ]]; then
        do_help     "$@"
        return      "$?"
    elif [[ "$1" == "clean" ]]; then
        do_clean    "$@"
        return      "$?"
    elif [[ "$1" == "encode" ]]; then
        do_encode    "$@"
        return      "$?"
    elif [[ "$1" == "format" ]]; then
        do_format   "$@"
        return      "$?"
    else
        echo    "Unsupported command '$1', type -h for help"
        return  1
    fi
}


main    "$@"
exit    "$?"
