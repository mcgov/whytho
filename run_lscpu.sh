#! /bin/bash

source ./common.sh
install_dependencies lscpu
sudo lscpu
sudo lscpu -e=CPU,BOGOMIPS,CORE,SOCKET,CLUSTER,NODE,BOOK,DRAWER,CACHE,POLARIZATION,ADDRESS,CONFIGURED,ONLINE,MHZ,MAXMHZ,MINMHZ