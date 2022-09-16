#! /bin/bash
source ./common.sh

install_dependencies lscpu gcc lsmem
install_dependencies libnuma-devel; install_dependencies numctl-libs || true

gcc memnuma.c -O1 -o memnuma --std=gnu11 -fno-aggressive-loop-optimizations -lnuma --debug -fsanitize=address

# some versions of lspcu don't support the -B flag
ARGS=`getconf -a | grep CACHE_SIZE  | grep -v LEVEL_4 | sort  | awk ' { print $2 } ' | tr "\n" " " `

# allow user args to replace the default cache sized args, passing 5 args will skip
# the pagemap parsing and printing
if [ "$#" -eq "1" ]; then
    echo "Appending arg $1..."
    ARGS="$ARGS $1"
elif [ "$#" -gt "4" ]; then
    echo "Warning, replacing cache arguments with arbitrary args passed to $0..."
    ARGS="${@:1}"
fi;
echo $ARGS
time for i in {0..10}; do sudo ./memnuma $ARGS ; done
rm ./memnuma
lsmem --output RANGE,SIZE,STATE,BLOCK,NODE,ZONES