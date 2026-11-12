#!/bin/sh
cd "$(dirname "$0")" || exit 1

docker build -t uclinux-buildenv:0.1 .
