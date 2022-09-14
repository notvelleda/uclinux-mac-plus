#!/bin/bash

# the part of the build script executed in the Docker container

set -e

make dep && make subdirs modules
m68k-elf-objcopy -O binary -j .text /linux/linux-2.0.x/linux /linux/images/linux.text
m68k-elf-objcopy -O binary -j .data /linux/linux-2.0.x/linux /linux/images/linux.data
cat images/linux.text images/linux.data > images/kernel.img
