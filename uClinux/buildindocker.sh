#!/bin/bash

# the part of the build script executed in the Docker container

set -e

mkdir -p images
make dep && make linux
cp linux-2.0.x/linux images/kernel
m68k-elf-strip images/kernel
