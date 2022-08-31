#! /bin/bash
test -d results || mkdir results
if [ -z $1 ]; then
    INFO_DEST="results/info_results.log"
else
    INFO_DEST="results/info_results-$1.log"
fi

touch $INFO_DEST
echo "Gathering info. NOTE: This will take a short while, just gathering info..."
ls run_*.sh | xargs -t -I % sh -c "./%; exit 0;" &> $INFO_DEST
if ! [ -e results/dmesg.log ]; 
then
    echo "$(hostname) ------------" >> results/dmesg.log
    sudo dmesg >> results/dmesg.log
fi
echo "All done with info gathering..."