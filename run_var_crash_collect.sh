#! /bin/bash

source ./common.sh

if [ -d /var/crash ] && [ `sudo ls /var/crash/*` ]; 
then
  echo "Found things in /var/crash, copying content to results"
  install_dependencies zip
  sudo zip "./results/var_crash.zip" /var/crash/*
  sudo chown -R $USER "./results/var_crash.zip"
else
  echo "Did not find anything in /var/crash (below)..."
  sudo ls -la /var/crash
fi
  
