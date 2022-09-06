<<<<<<< HEAD
#! /bin/bash

source ./common.sh
install_dependencies lscpu
sudo lscpu
sudo lscpu -e=CPU,BOGOMIPS,CORE,SOCKET,CLUSTER,NODE,BOOK,DRAWER,CACHE,POLARIZATION,ADDRESS,CONFIGURED,ONLINE,MHZ,MAXMHZ,MINMHZ
=======
install_dependencies lscpu
lscpu
>>>>>>> 5b79556270156c4e090648e785a7ccd1b121d362
