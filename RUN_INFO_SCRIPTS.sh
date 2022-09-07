#! /bin/bash
source ./common.sh

if [ -z $1 ]; then
    INFO_DEST="results/info_results.log"
else
    INFO_DEST="results/info_results-$1.log"
fi


touch $INFO_DEST
echo "Gathering info. NOTE: This will take a short while, just gathering info..."
ls run_*.sh | xargs -n 1 -t -I % sh -c "./%; $WHYTHO_DEBUG_ACTIONS" &> $INFO_DEST
if ! [ -e results/dmesg.log ];
then
    echo "$(hostname) ------------" >> results/dmesg.log
    sudo dmesg >> results/dmesg.log
fi
echo "All done with info gathering..."