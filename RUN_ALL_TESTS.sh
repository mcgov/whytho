#! /bin/bash
test -d results || mkdir results
touch results/all_results.log
echo "NOTE: This will take a while. Now might be a good time to get some coffee?"
ls run_*.sh | xargs -t -I % sh -c "./%; exit 0;" &> results/all_results.log
echo "All done :) results are in results/all_results.log"
echo "$(hostname) ------------" >> results/dmesg.log
sudo dmesg >> results/dmesg.log