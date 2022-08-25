#! /bin/bash

source ./common.sh
install_dependencies gcc

gcc memhammer.c -o memhammer -O1 --std=c17
time ./memhammer
rm ./memhammer
