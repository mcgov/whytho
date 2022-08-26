source ./common.sh

install_dependencies lscpu gcc

gcc mnuma.c -O0 -o mnuma --std=gnu11
ARGS=`lscpu -C -B | tail -4 | awk ' { print $2 } ' | tr "\n" " " `
./mnuma $ARGS
rm ./mnuma