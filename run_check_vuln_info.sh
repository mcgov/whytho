#! /bin/bash

source ./common.sh
install_dependencies lscpu
lscpu
sudo grep . /sys/devices/system/cpu/vulnerabilities/*