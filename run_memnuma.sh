source ./common.sh

install_dependencies lscpu gcc lsmem

gcc memnuma.c -O1 -o memnuma --std=gnu11 -fno-aggressive-loop-optimizations --debug

# some versions of lspcu don't support the -B flag
ARGS=`getconf -a | grep CACHE_SIZE  | grep -v LEVEL_4 | sort  | awk ' { print $2 } ' | tr "\n" " " `
echo $ARGS
sudo ./memnuma $ARGS && rm ./memnuma
lsmem --output-all