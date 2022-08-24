#! /bin/sh

time for i in {0..10000000}; do trap exit SIGWINCH; done