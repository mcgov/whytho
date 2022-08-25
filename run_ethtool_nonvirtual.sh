#! /bin/bash
ls /sys/class/net | grep -v "$(ls /sys/devices/virtual/net)" | sudo xargs -t -I % sh -c "ethtool %; ethtool -S %"
