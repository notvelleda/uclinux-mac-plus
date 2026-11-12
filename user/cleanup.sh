#!/bin/bash
set -e

cd "$(dirname "$0")"

ROOTDIR="$(pwd)/rootdir"

rm -fr "${ROOTDIR}"

makeClean() {
  dir=${1}
  wd=$(pwd)

  cd "${dir}"
  make clean
  cd "${wd}"
}

makeClean busybox
rm busybox/cat
rm busybox/msh
rm busybox/df
rm busybox/getopt
rm busybox/grep
rm busybox/pidof
rm busybox/hostname
rm busybox/sed
rm busybox/usleep

rm busybox/ps
rm busybox/halt
rm busybox/insmod
rm busybox/rmmod
rm busybox/modprobe
rm busybox/makedevs
rm busybox/reboot

rm busybox/test

rm busybox/ar
rm busybox/cut
rm busybox/dc
rm busybox/du
rm busybox/expr
rm busybox/find
rm busybox/free
rm busybox/head
rm busybox/id
rm busybox/kill
rm busybox/md5sum
rm busybox/printf
rm busybox/reset
rm busybox/tail
rm busybox/tee
rm busybox/time
rm busybox/uptime
rm busybox/wc


makeClean tinylogin
makeClean init
makeClean levee
makeClean sh_utils
makeClean file_utils
makeClean sys_utils
makeClean mount
makeClean mplay
makeClean ../bootloader

rm root.img
