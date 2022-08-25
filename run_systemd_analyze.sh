#! /bin/bash
sudo systemd-analyze blame # output which systemd service took the most time to get started
sudo systemd-analyze critical-chain # more output about systemd service start up times
