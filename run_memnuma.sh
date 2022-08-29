source ./common.sh

install_dependencies lscpu gcc lsmem

gcc memnuma.c -O1 -o memnuma --std=gnu11 -fno-aggressive-loop-optimizations --debug
ARGS=`lscpu -C -B | tail -4 | awk ' { print $2 } ' | tr "\n" " " `
lscpu -C -B && sudo ./memnuma $ARGS && rm ./memnuma
lsmem --output-all