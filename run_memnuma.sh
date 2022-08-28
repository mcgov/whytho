source ./common.sh

install_dependencies lscpu gcc lsmem

gcc memnuma.c -O1 -o memnuma --std=gnu11 -fno-aggressive-loop-optimizations
ARGS=`lscpu -C -B | tail -4 | awk ' { print $2 } ' | tr "\n" " " `
sudo ./memnuma $ARGS
rm ./memnuma
lsmem --output-all