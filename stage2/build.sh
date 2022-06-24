#!/bin/sh
UCLINUX_DIR=$(pwd)/../uClinux
set -e
echo "Building..."
docker run --user $(id -u):$(id -u) --mount type=bind,source="$(pwd)",target=/build --mount type=bind,source="$UCLINUX_DIR",target=/linux --workdir /build -e "GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'" -e "TERM=xterm-256color" -t uclinux-buildenv:0.1 bash -c "make -f Makefile clean && make -f Makefile UCLINUX_DIR=/linux CC=m68k-elf-gcc"
