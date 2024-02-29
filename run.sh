#!/bin/sh
cd /home/theodoros/Documents/PersonalRepos/PersonalOS
make clean
./build.sh

qemu-system-i386 -hda ./bin/os.bin
