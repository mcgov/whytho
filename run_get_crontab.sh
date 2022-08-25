#! /bin/bash
sudo cut -d: -f1 /etc/passwd | xargs -t -I % sh -c "sudo -u % crontab -l"