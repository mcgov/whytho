#! /bin/bash
uname -a
lsmod | awk '{print $1}' | grep hv_  | xargs -t -I % modinfo %
sudo yum list installed | grep hyperv