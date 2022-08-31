#! /bin/bash
test -d results || mkdir results
if [ -z $1 ]; then
    INFO_DEST="results/perf_results.log"
else
    INFO_DEST="results/perf_results-$1.log"
fi

touch $INFO_DEST
echo "Running perf benchmarks. NOTE: This will take a while. Now might be a good time to get some coffee?"
ls perf_run_*.sh | xargs -t -I % sh -c "./%; exit 0;" &> $INFO_DEST
if ! [ -e results/dmesg.log ];
then
    echo "$(hostname) ------------" >> results/dmesg.log
    sudo dmesg >> results/dmesg.log
fi
echo "All done with perf benchmarks..."