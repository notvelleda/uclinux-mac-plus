#!/bin/bash
set -e
set -o pipefail
cd "$(dirname "$0")"

if ! docker image exists uclinux-buildenv:0.1 2>/dev/null && [ -z "$(docker images -q uclinux-buildenv:0.1 2>/dev/null)" ]; then
    echo "### Step 0/3: build environment image"
    ./buildenv/build.sh
fi

echo "### Step 1/3: kernel -> uClinux/images/kernel"
./uClinux/build.sh

echo "### Step 2/3: userland programs -> user/rootdir"
./user/build.sh

echo "### Step 3/3: disk image -> user/root.img"
./user/buildfs.sh
