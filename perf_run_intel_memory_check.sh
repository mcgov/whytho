#! /bin/bash
mkdir mlc_dir
cd ./mlc_dir
wget https://downloadmirror.intel.com/736634/mlc_v3.9a.tgz
tar xvf ./mlc_v3.9a.tgz
SAVED_NR_HUGEPAGE=$(sudo cat /proc/sys/vm/nr_hugepages)
echo 4000 | sudo tee /proc/sys/vm/nr_hugepages
cat /proc/interrupts
./Linux/mlc
cat /proc/interrupts
cd ..
rm -rf ./mlc_dir
echo $SAVED_NR_HUGEPAGE | sudo tee /proc/sys/vm/nr_hugepages