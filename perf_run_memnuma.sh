#! /bin/bash

source ./common.sh

install_dependencies lscpu gcc lsmem libnuma-devel

gcc memnuma.c -O1 -o memnuma --std=gnu11 -fno-aggressive-loop-optimizations -lnuma --debug

# some versions of lspcu don't support the -B flag
ARGS=`getconf -a | grep CACHE_SIZE  | grep -v LEVEL_4 | sort  | awk ' { print $2 } ' | tr "\n" " " `
echo $ARGS
time for i in {0..10}; do sudo ./memnuma $ARGS ; done
rm ./memnuma
lsmem --output RANGE,SIZE,STATE,BLOCK,NODE,ZONES