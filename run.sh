#!/bin/sh
cd /home/theodoros/Documents/PersonalRepos/PersonalOS
make clean
./build.sh
cd bin
qemu-system-i386 -hda ./os.bin
