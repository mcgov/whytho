#! /bin/bash

source ./common.sh

# store things in a tmp directory
mkdir mlc_dir
cd ./mlc_dir
# download the resources for the memory check test
wget https://downloadmirror.intel.com/736634/mlc_v3.9a.tgz
tar xvf ./mlc_v3.9a.tgz
# save the original hugepage info
SAVED_NR_HUGEPAGE=$(sudo cat /proc/sys/vm/nr_hugepages)
# enable a bunch of hugepages
echo 4000 | sudo tee /proc/sys/vm/nr_hugepages
# note interrupts before test

start_interrupts_log
# run the tool
./Linux/mlc
# note interrupts after test
compare_interrupts_logs

# remove tmp assets
cd ..
rm -rf ./mlc_dir
# restore hugepage setting
echo $SAVED_NR_HUGEPAGE | sudo tee /proc/sys/vm/nr_hugepages
