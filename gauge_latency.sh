source ./common.sh

install_sysbench

sysbench memory  --memory-access-mode=rnd --memory-total-size=12G --memory-block-size=512M run

