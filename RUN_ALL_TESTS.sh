#! /bin/bash
source ./common.sh

FILE_TAG="`hostname`-`date +%Y-%m-%d-%H_%M_%S-%s`"
echo "Running all tests..."
sudo ./RUN_INFO_SCRIPTS.sh $FILE_TAG
sudo ./RUN_PERF_BENCHMARKS.sh $FILE_TAG

# warn about packages installed
if [ -e ./packages_added.log ];
then
    echo "NOTE: tests installed the following packages that may require cleanup..."
    cat ./packages_added.log
fi

./ZIP_RESULTS.sh $FILE_TAG $@