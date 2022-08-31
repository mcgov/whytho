#! /bin/bash

source ./common.sh
install_dependencies gcc

gcc memhammer.c -o memhammer -O2  --debug --std=gnu11
time ./memhammer
rm ./memhammer
