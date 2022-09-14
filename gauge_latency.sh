source ./common.sh

install_sysbench

sysbench memory  --memory-access-mode=rnd --memory-total-size=28G --memory-block-size=512M run

