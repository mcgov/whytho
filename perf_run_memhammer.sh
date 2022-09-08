#! /bin/bash

source ./common.sh
install_dependencies gcc

gcc memhammer.c -o memhammer -O2  --debug --std=gnu11

if [ -n "$1" ]; then
    time ./memhammer $1
else
    time ./memhammer
fi

rm ./memhammer
