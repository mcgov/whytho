#! /bin/bash
source ./common.sh
install_dependencies sysbench curl

# check if it was missing, if yes then use their package install script.
if [ -z `which sysbench` ];
then
    curl -s https://packagecloud.io/install/repositories/akopytov/sysbench/script.rpm.sh | sudo bash
fi;

install_dependencies sysbench
if [ -z `which sysbench` ];
then
    exit 0;
fi;

# get an interrupt baseline
cat /proc/interrupts

# - File I/O test
sysbench fileio --file-test-mode=seqwr run
sysbench fileio --file-test-mode=seqwr cleanup
cat /proc/interrupts

# - CPU performance test
sysbench cpu run
cat /proc/interrupts

 # Memory functions speed test
sysbench memory run
sysbench memory run --memory-hugetlb=on
cat /proc/interrupts

#threads
sysbench threads run
cat /proc/interrupts

#threads
sysbench mutex run
cat /proc/interrupts