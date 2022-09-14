#!/bin/sh
set -e
echo "Building kernel..."
docker run --user $(id -u):$(id -u) --mount type=bind,source="$(pwd)",target=/linux --workdir /linux -e "GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'" -e "TERM=xterm-256color" -t uclinux-buildenv:0.1 bash buildindocker.sh
