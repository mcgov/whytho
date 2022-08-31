#! /bin/bash
source ./common.sh

FILE_TAG="`hostname`-`date +%Y-%M-%d-%H_%M_%S-%s`"
echo "Running all tests..."
sudo ./RUN_INFO_SCRIPTS.sh $FILE_TAG
sudo ./RUN_PERF_BENCHMARKS.sh $FILE_TAG

# save results into a .zip
install_dependencies zip
sudo chown -R $USER ./results
zip "$FILE_TAG.zip" ./results/*

echo "All done :) results are in $FILE_TAG.zip"

# warn about packages installed
if [ -e ./packages_added.log ];
then
    echo "NOTE: tests installed the following packages that may require cleanup..."
    cat ./packages_added.log
fi

#clear results
rm -rf ./results