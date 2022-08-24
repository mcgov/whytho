gcc mem_cronch.c -o cronch -O0 --debug
time ./cronch `free -g | grep Mem | awk '{ print $4 }'`
rm ./cronch
