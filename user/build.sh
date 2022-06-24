#!/bin/bash

set -e

UCLINUX_DIR="$(pwd)/../uClinux"
CONT_UCLINUX_DIR="/linux"
CROSS="m68k-elf-"
CFLAGS="-m68000 -Os -g -fomit-frame-pointer -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED -D_POSIX_VERSION -I${CONT_UCLINUX_DIR}/lib/libc/include -I${CONT_UCLINUX_DIR}/lib/libm -I${CONT_UCLINUX_DIR}/lib/libcrypt_old -I${CONT_UCLINUX_DIR} -fno-builtin -msep-data -I${CONT_UCLINUX_DIR}/linux-2.0.x/include"
LDFLAGS="${CFLAGS} -Wl,-elf2flt -Wl,-move-rodata -L${CONT_UCLINUX_DIR}/lib/libc/. -L${CONT_UCLINUX_DIR}/lib/libc/lib -L${CONT_UCLINUX_DIR}/lib/libm -L${CONT_UCLINUX_DIR}/lib/libnet -L${CONT_UCLINUX_DIR}/lib/libdes -L${CONT_UCLINUX_DIR}/lib/libaes -L${CONT_UCLINUX_DIR}/lib/libpcap -L${CONT_UCLINUX_DIR}/lib/libssl -L${CONT_UCLINUX_DIR}/lib/libcrypt_old -L${CONT_UCLINUX_DIR}/prop/libsnapgear++ -L${CONT_UCLINUX_DIR}/prop/libsnapgear -L${CONT_UCLINUX_DIR}/lib/zlib"
LDLIBS="-lc -lcrypt_old"

ROOTDIR="$(pwd)/rootdir"

makeIn() {
    dir=${1}
    target=${2}
    before=${3:-echo uwu}
    wd=$(pwd)

    echo " ==== making ${target} in ${dir}..."
    cd "${dir}"
    docker run \
        --user $(id -u):$(id -u) \
        --mount type=bind,source="$(pwd)",target=/build \
        --mount type=bind,source="${UCLINUX_DIR}",target="${CONT_UCLINUX_DIR}" \
        --workdir /build \
        -t uclinux-buildenv:0.1 \
        sh -c "$before ; make CROSS=\"${CROSS}\" CC=\"${CROSS}gcc\" CFLAGS=\"${CFLAGS}\" LDFLAGS=\"${LDFLAGS}\" LDLIBS=\"${LDLIBS}\" ${target}"
    cd "${wd}"
}

mkdir -p "${ROOTDIR}"
mkdir -p "${ROOTDIR}/bin"
mkdir -p "${ROOTDIR}/dev"
mkdir -p "${ROOTDIR}/etc"
mkdir -p "${ROOTDIR}/mnt"
mkdir -p "${ROOTDIR}/proc"
mkdir -p "${ROOTDIR}/root"
mkdir -p "${ROOTDIR}/sbin"
mkdir -p "${ROOTDIR}/usr"
mkdir -p "${ROOTDIR}/var"
mkdir -p "${ROOTDIR}/var/tmp"

makeIn init all
cp init/init "${ROOTDIR}/sbin/"

makeIn busybox applet_source_list
makeIn busybox busybox
makeIn busybox busybox.links
cd busybox
./install.sh "${ROOTDIR}"
cd ..

makeIn levee vi
cp levee/vi "${ROOTDIR}/bin/"

makeIn tinylogin/pwd_grp all
makeIn tinylogin applet_source_list
makeIn tinylogin tinylogin
makeIn tinylogin tinylogin.links
cd tinylogin
./install.sh "${ROOTDIR}"
cd ..

makeIn ll all
cp ll/ll "${ROOTDIR}/bin/"
