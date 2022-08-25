#! /bin/bash
source ./common.sh

echo "sh -c cat waagent.conf"
sudo cat /etc/waagent.conf

get_content_within "/etc/logrotate.d/waagent*"

# get configuration and other info for waagent
get_content_within /var/lib/waagent