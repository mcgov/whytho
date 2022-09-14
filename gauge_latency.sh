source ./common.sh

install_dependencies gcc wget nproc

install_sysbench

sysbench memory run --threads=`nproc`

