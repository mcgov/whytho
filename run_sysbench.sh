#! /bin/bash
source common.h
install_dependencies sysbench

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