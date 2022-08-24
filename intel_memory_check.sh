#! /bin/sh
wget https://downloadmirror.intel.com/736634/mlc_v3.9a.tgz
mkdir mlc_dir
cd mlc_dir
tar xvf ../mlc_v3.9a.tgz
SAVED_NR_HUGEPAGE=$(sudo cat /proc/sys/vm/nr_hugepages)
echo 4000 | sudo tee /proc/sys/vm/nr_hugepages
./Linux/mlc
cd ..
rm -rf mlc_dir
rm ./mlc_v3.9a.tgz
echo $SAVED_NR_HUGEPAGE | sudo tee /proc/sys/vm/nr_hugepages