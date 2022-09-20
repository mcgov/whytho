#! /bin/bash

source ./common.sh

if [ -z "$1" ] && [ "$1" != "--retain" ]; then
    FILE_TAG="`hostname`-`date +%Y-%m-%d-%H_%M_%S-%s`"
else
    FILE_TAG="$1"
fi

# save results into a .zip
install_dependencies zip
sudo chown -R $USER ./results
#zip results...
zip "$FILE_TAG.zip" ./results/*
echo "All done :) results are in $FILE_TAG.zip"

#if arguments contain --retain don't delete results
for arg in $@; do
    if [ "$arg" == "--retain" ]; then
        exit 0
    fi;
done;

# else clear results
rm -rf ./results