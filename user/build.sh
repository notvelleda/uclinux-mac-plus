#!/bin/bash

set -e

UCLINUX_DIR="$(pwd)/../uClinux"
CONT_UCLINUX_DIR="/linux"
CROSS="m68k-elf-"
CFLAGS="-m68000 -Os -g -fomit-frame-pointer -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED -D_POSIX_VERSION -I${CONT_UCLINUX_DIR}/lib/libc/include -I${CONT_UCLINUX_DIR}/lib/libm -I${CONT_UCLINUX_DIR}/lib/libcrypt_old -I${CONT_UCLINUX_DIR} -fno-builtin -msep-data -I${CONT_UCLINUX_DIR}/linux-2.0.x/include"
LDFLAGS="${CFLAGS} -Wl,-elf2flt -Wl,-move-rodata -L${CONT_UCLINUX_DIR}/lib/libc/. -L${CONT_UCLINUX_DIR}/lib/libc/lib -L${CONT_UCLINUX_DIR}/lib/libm -L${CONT_UCLINUX_DIR}/lib/libnet -L${CONT_UCLINUX_DIR}/lib/libdes -L${CONT_UCLINUX_DIR}/lib/libaes -L${CONT_UCLINUX_DIR}/lib/libpcap -L${CONT_UCLINUX_DIR}/lib/libssl -L${CONT_UCLINUX_DIR}/lib/libcrypt_old -L${CONT_UCLINUX_DIR}/prop/libsnapgear++ -L${CONT_UCLINUX_DIR}/prop/libsnapgear -L${CONT_UCLINUX_DIR}/lib/zlib"
LDLIBS="-lc -lcrypt_old"

ROOTDIR="$(pwd)/rootdir"

makeFileIn() {
    makefile=${1}
    dir=${2}
    target=${3}
    before=${4:-echo uwu}
    wd=$(pwd)

    echo " ==== making ${target} in ${dir} with ${makefile}..."
    cd "${dir}"
    docker run \
        --user $(id -u):$(id -u) \
        --mount type=bind,source="$(pwd)",target=/build \
        --mount type=bind,source="${UCLINUX_DIR}",target="${CONT_UCLINUX_DIR}" \
        --workdir /build \
        -t uclinux-buildenv:0.1 \
        sh -c "$before ; make -f \"${makefile}\" CROSS=\"${CROSS}\" CC=\"${CROSS}gcc\" LD=\"${CROSS}ld\" CFLAGS=\"${CFLAGS}\" LDFLAGS=\"${LDFLAGS}\" LDLIBS=\"${LDLIBS}\" ${target}"
    cd "${wd}"
}

makeIn() {
    makeFileIn Makefile $@
}

copy() {
    echo "copying ${1} -> ${2}"
    cp --remove-destination "${1}" "${2}"
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

#makeIn ../uClinux/lib/libc all

makeIn busybox applet_source_list
makeIn busybox libbb.a
#makeIn busybox busybox
#makeIn busybox busybox.links
#cd busybox
#./install.sh "${ROOTDIR}"
#cd ..

makebb() {
    makeFileIn Makefile.standalone busybox $1
    copy busybox/$1 $2
}

makebb msh "${ROOTDIR}/bin/"
ln -sf msh "${ROOTDIR}/bin/sh"
makebb df "${ROOTDIR}/bin/"
makebb getopt "${ROOTDIR}/bin/"
makebb grep "${ROOTDIR}/bin/"
makebb pidof "${ROOTDIR}/bin/"
makebb hostname "${ROOTDIR}/bin/"
makebb sed "${ROOTDIR}/bin/"
makebb usleep "${ROOTDIR}/bin/"
makebb ps "${ROOTDIR}/bin/"

makebb halt "${ROOTDIR}/sbin/"
makebb insmod "${ROOTDIR}/sbin/"
makebb rmmod "${ROOTDIR}/sbin/"
makebb modprobe "${ROOTDIR}/sbin/"
makebb makedevs "${ROOTDIR}/sbin/"
makebb reboot "${ROOTDIR}/sbin/"

makebb test "${ROOTDIR}/usr/bin/test"
ln -sf test "${ROOTDIR}/usr/bin/["
makebb ar "${ROOTDIR}/usr/bin/"
#makebb basename "${ROOTDIR}/usr/bin/"
#makebb clear "${ROOTDIR}/usr/bin/"
#makebb cmp "${ROOTDIR}/usr/bin/"
makebb cut "${ROOTDIR}/usr/bin/"
makebb dc "${ROOTDIR}/usr/bin/"
#makebb dirname "${ROOTDIR}/usr/bin/"
makebb du "${ROOTDIR}/usr/bin/"
makebb expr "${ROOTDIR}/usr/bin/"
makebb find "${ROOTDIR}/usr/bin/"
makebb free "${ROOTDIR}/usr/bin/"
makebb head "${ROOTDIR}/usr/bin/"
makebb id "${ROOTDIR}/usr/bin/"
makebb kill "${ROOTDIR}/usr/bin/killall"
makebb md5sum "${ROOTDIR}/usr/bin/"
makebb printf "${ROOTDIR}/usr/bin/"
makebb reset "${ROOTDIR}/usr/bin/"
makebb tail "${ROOTDIR}/usr/bin/"
makebb tee "${ROOTDIR}/usr/bin/"
makebb time "${ROOTDIR}/usr/bin/"
#makebb tr "${ROOTDIR}/usr/bin/"
makebb uptime "${ROOTDIR}/usr/bin/"
makebb wc "${ROOTDIR}/usr/bin/"
#makebb which "${ROOTDIR}/usr/bin/"
#makebb whoami "${ROOTDIR}/usr/bin/"
#makebb xargs "${ROOTDIR}/usr/bin/"
#makebb yes "${ROOTDIR}/usr/bin/"

makeIn init all
copy init/init "${ROOTDIR}/sbin/"

makeIn levee vi
copy levee/vi "${ROOTDIR}/bin/"

makeIn tinylogin/pwd_grp all
makeIn tinylogin applet_source_list
makeIn tinylogin tinylogin
makeIn tinylogin tinylogin.links
cd tinylogin
./install.sh "${ROOTDIR}"
cd ..

makeIn sh_utils all
cd sh_utils
make install INSTALL=install DESTDIR="${ROOTDIR}"
cd ..

makeIn file_utils all
cd file_utils
make install INSTALL=install DESTDIR="${ROOTDIR}"
cd ..

makeIn sys_utils all
cd sys_utils
make install INSTALL=install DESTDIR="${ROOTDIR}"
cd ..

makeIn mount all
copy mount/mount "${ROOTDIR}/bin/"
copy mount/umount "${ROOTDIR}/bin/"

moveToUsr() {
    mv "${ROOTDIR}/bin/$1" "${ROOTDIR}/usr/bin/$1"
}

moveToUsr basename
moveToUsr clear
moveToUsr cmp
moveToUsr dirname
moveToUsr tr
moveToUsr which
moveToUsr whoami
moveToUsr xargs
moveToUsr yes

chmod 755 ${ROOTDIR}/bin/*
chmod 755 ${ROOTDIR}/usr/bin/*
