#! /bin/bash

# very basic check of latency, prints "pass" "fail" or "error" if no data was present.

source ./common.sh

install_sysbench

sysbench memory  --memory-access-mode=rnd --memory-total-size=6G --memory-block-size=512M run &> sysbench_result.log

#echo sysbench results
cat sysbench_result.log


#check 95th percentile latency
PERCENTILE_CHECK="`cat sysbench_result.log | grep \"95th percentile:\"`"

# if latency is higher than an abritrary threshold, report fail
# threshold is (ms) reported by sysbench to access memory-block-size
# should be tailored to the system, it will vary between hardware.
echo "Scoring latency..."
if [ -n "$PERCENTILE_CHECK" ]; then
    RESULT=`echo "$PERCENTILE_CHECK" | awk '{  print  ($3  > 3500.0 ? "fail" : "pass")  }'`
#                                                              ^ threshold (ms)
else
    RESULT="error"
fi
echo "$RESULT"
case $RESULT in
    pass)
        exit 0
    ;;
    fail)
        exit -1
    ;;
    error)
        exit -128
    ;;
    *)
        exit -255
    ;;
esac
