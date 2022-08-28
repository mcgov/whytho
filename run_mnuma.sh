source ./common.sh

install_dependencies lscpu gcc lsmem

gcc mnuma.c -O1 -o mnuma --std=gnu11
ARGS=`lscpu -C -B | tail -4 | awk ' { print $2 } ' | tr "\n" " " `
sudo ./mnuma $ARGS
rm ./mnuma
lsmem --output-all