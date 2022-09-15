source ./common.sh

install_sysbench

sysbench memory  --memory-access-mode=rnd --memory-total-size=6G --memory-block-size=512M run

