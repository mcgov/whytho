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
start_interrupts_log

# - File I/O test
echo "sysbench running fileio ------------"
sysbench fileio --file-test-mode=seqwr run
sysbench fileio --file-test-mode=seqwr cleanup

compare_interrupts_logs

# - CPU performance test
echo "sysbench running cpu ------------"
sysbench cpu run

compare_interrupts_logs

# Memory functions speed test
echo "sysbench running memory ------------"
sysbench memory run
sysbench memory run --memory-hugetlb=on
compare_interrupts_logs

#threads
echo "sysbench running threads ------------"
sysbench threads run
compare_interrupts_logs

#threads
echo "sysbench running mutex ------------"
sysbench mutex run
compare_interrupts_logs

cleanup_interrupts_logs