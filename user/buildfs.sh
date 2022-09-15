#!/bin/sh

FILE=root.img
MOUNTDIR=__mountdir
DEVICE=/dev/loop0
FSDIR=rootdir
OVERLAYDIR=rootdir-overlay
SIZE=64M

unmount() {
    sudo umount "$MOUNTDIR"
    sudo losetup -d "$DEVICE"
    rm -r "$MOUNTDIR"
}

quit() {
    if [[ "$1" == "true" ]]; then
        echo "Quitting on error..."
    fi
    unmount
    exit
}

echo "Setting up, mounting..."
mkdir -p "$MOUNTDIR"

if [[ "$1" == "clean" ]]; then
    rm "$FILE"
fi

if [ ! -f "$FILE" ]; then
    echo "File doesn't exist, creating it..."
    truncate -s "$SIZE" "$FILE" || quit true
    sudo losetup "$DEVICE" "$FILE" || quit true
    sudo mkfs.ext2 -O none -I 128 "$DEVICE" || quit true
else
    sudo losetup "$DEVICE" "$FILE" || quit true
fi

sudo mount "$DEVICE" "$MOUNTDIR" || quit true

echo "Copying files..."

sudo rm -r $MOUNTDIR/*

sudo cp -rv $FSDIR/* "$MOUNTDIR" || quit true
sudo cp -rv $OVERLAYDIR/* "$MOUNTDIR" || quit true

sudo ln -s /var/tmp "$MOUNTDIR/tmp"

echo "Creating device nodes..."

sudo mknod $MOUNTDIR/dev/mem c 1 1
sudo mknod $MOUNTDIR/dev/kmem c 1 2
sudo mknod $MOUNTDIR/dev/null c 1 3
sudo mknod $MOUNTDIR/dev/port c 1 4
sudo mknod $MOUNTDIR/dev/zero c 1 5
#sudo mknod $MOUNTDIR/dev/core c 1 6
sudo mknod $MOUNTDIR/dev/full c 1 7
sudo mknod $MOUNTDIR/dev/random c 1 8
sudo mknod $MOUNTDIR/dev/urandom c 1 9

sudo mknod $MOUNTDIR/dev/ptyp0 c 2 0
sudo mknod $MOUNTDIR/dev/ptyp1 c 2 1
sudo mknod $MOUNTDIR/dev/ptyp2 c 2 2
sudo mknod $MOUNTDIR/dev/ptyp3 c 2 3
sudo mknod $MOUNTDIR/dev/ptyp4 c 2 4
sudo mknod $MOUNTDIR/dev/ptyp5 c 2 5
sudo mknod $MOUNTDIR/dev/ptyp6 c 2 6
sudo mknod $MOUNTDIR/dev/ptyp7 c 2 7
sudo mknod $MOUNTDIR/dev/ptyp8 c 2 8
sudo mknod $MOUNTDIR/dev/ptyp9 c 2 9
sudo mknod $MOUNTDIR/dev/ptypa c 2 10
sudo mknod $MOUNTDIR/dev/ptypb c 2 11
sudo mknod $MOUNTDIR/dev/ptypc c 2 12
sudo mknod $MOUNTDIR/dev/ptypd c 2 13
sudo mknod $MOUNTDIR/dev/ptype c 2 14
sudo mknod $MOUNTDIR/dev/ptypf c 2 15

sudo mknod $MOUNTDIR/dev/ttyp0 c 3 0
sudo mknod $MOUNTDIR/dev/ttyp1 c 3 1
sudo mknod $MOUNTDIR/dev/ttyp2 c 3 2
sudo mknod $MOUNTDIR/dev/ttyp3 c 3 3
sudo mknod $MOUNTDIR/dev/ttyp4 c 3 4
sudo mknod $MOUNTDIR/dev/ttyp5 c 3 5
sudo mknod $MOUNTDIR/dev/ttyp6 c 3 6
sudo mknod $MOUNTDIR/dev/ttyp7 c 3 7
sudo mknod $MOUNTDIR/dev/ttyp8 c 3 8
sudo mknod $MOUNTDIR/dev/ttyp9 c 3 9
sudo mknod $MOUNTDIR/dev/ttypa c 3 10
sudo mknod $MOUNTDIR/dev/ttypb c 3 11
sudo mknod $MOUNTDIR/dev/ttypc c 3 12
sudo mknod $MOUNTDIR/dev/ttypd c 3 13
sudo mknod $MOUNTDIR/dev/ttype c 3 14
sudo mknod $MOUNTDIR/dev/ttypf c 3 15

sudo mknod $MOUNTDIR/dev/tty0 c 4 0

sudo mknod $MOUNTDIR/dev/tty c 5 0
sudo mknod $MOUNTDIR/dev/console c 5 1

sudo mknod $MOUNTDIR/dev/sound c 14 0

sudo mknod $MOUNTDIR/dev/fb0 c 29 0
sudo mknod $MOUNTDIR/dev/fb0autodetect c 29 1
sudo mknod $MOUNTDIR/dev/fb0user0 c 29 24
sudo mknod $MOUNTDIR/dev/fb0user1 c 29 25
sudo mknod $MOUNTDIR/dev/fb0user2 c 29 26
sudo mknod $MOUNTDIR/dev/fb0user3 c 29 27
sudo mknod $MOUNTDIR/dev/fb0user4 c 29 28
sudo mknod $MOUNTDIR/dev/fb0user5 c 29 29
sudo mknod $MOUNTDIR/dev/fb0user6 c 29 30
sudo mknod $MOUNTDIR/dev/fb0user7 c 29 31

makescsi() {
    id=$2
    sudo mknod $MOUNTDIR/dev/sd${1} b 8 $id
    for i in $(seq 15); do
        id=$(expr $id + 1)
        sudo mknod $MOUNTDIR/dev/sd$1$i b 8 $id
    done
}

makescsi a 0
makescsi b 16
makescsi c 32
makescsi d 48
makescsi e 64
makescsi f 80
makescsi g 96

echo "Unmounting..."
unmount

echo "Done"
