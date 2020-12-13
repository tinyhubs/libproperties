#!/usr/bin/bash

SELFDIR=$(realpath $(dirname "$0"))

function do_help()
{
    echo    "Usage:"
    echo    "   build.sh [debug|release]"
    echo    "   build.sh [compile] [debug|release]"
    echo    "   build.sh package"
    echo    "   build.sh test"
    echo    "   build.sh format"
    echo    "   build.sh encode"
    echo    "   build.sh help|-h|--help"
    return  0
}

#   $1  compile
#   $2  mode(debug or release)
function do_compile()
{
    echo    "TODO do_compile"
}

function do_package()
{
    echo    "TODO do_package"
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

function main()
{
    if   [[ "$1" == "debug" ]] || [[ "$1" == "release" ]]; then
        do_compile  "compile"   "$@"
    elif [[ "$1" == "compile" ]]; then
        do_compile  "$@"
    elif [[ "$1" == "test" ]]; then
        do_test  "$@"
    elif [[ "$1" == "package" ]]; then
        do_package  "$@"
    elif [[ "$1" == "help" ]] || [[ "$1" == "-h" ]] || [[ "$1" == "--help" ]]; then
        do_package  "$@"
    else
        echo    "Unsupported command '$1', type -h for help"
        return  1
    fi
}


main    "$@"
exit    "$?"
