#! /bin/bash

source ./common.sh
install_dependencies gcc

gcc memhammer.c -o memhammer -O0 --debug --std=c17
time ./memhammer
rm ./memhammer
