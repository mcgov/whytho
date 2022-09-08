#! /bin/bash

source ./common.sh
install_dependencies lscpu
sudo lscpu
sudo lscpu -a -e
