source ./common.sh

install_dependencies gcc wget

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

cd ..

# run the tool and prepare the results to be passed to the helper c program
LATENCIES=`./mlc_dir/Linux/mlc --loaded_latency | grep -P "[0-9]+\s+[0-9]+\.[0-9]+\s+[0-9]+\.[0-9]+" | awk '{print $2}' | tr '\n' ' '`



gcc ./gauge_latency_helper.c -o gauge_latency_helper --std=gnu11
./gauge_latency_helper $LATENCIES

