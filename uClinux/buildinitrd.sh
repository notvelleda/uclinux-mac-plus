#!/bin/bash
shopt -s extglob

echo "Building kernel..."
docker run --user $(id -u):$(id -u) --mount type=bind,source="$(pwd)",target=/linux --workdir /linux -e "GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'" -e "TERM=xterm-256color" -t uclinux-buildenv:0.1 bash -c "make dep && make romfs" || exit

FILE=images/initrd_uncompressed.img
COMPRESSEDFILE=images/initrd.img
MOUNTDIR=mount
DEVICE=/dev/loop0
FSDIR=romfs
OVERLAYDIR=romfs-overlay

unmount() {
    sudo umount "$MOUNTDIR"
    sudo losetup -d "$DEVICE"
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
    rm "$COMPRESSEDFILE"
    rm "$FILE"
fi

if [ ! -f "$FILE" ]; then
    echo "File doesn't exist, creating it..."
    truncate -s 384k "$FILE" || quit true
    sudo losetup "$DEVICE" "$FILE" || quit true
    sudo mkfs.ext2 -O none -I 128 "$DEVICE" || quit true
else
    sudo losetup "$DEVICE" "$FILE" || quit true
fi

sudo mount "$DEVICE" "$MOUNTDIR" || quit true

echo "Copying files..."

sudo rm -r $MOUNTDIR/*

sudo cp -rv $FSDIR/!(dev*|tmp*) "$MOUNTDIR" || quit true
if [[ -e "$OVERLAYDIR" ]]; then
	sudo cp -rv $OVERLAYDIR/* "$MOUNTDIR" || quit true
fi

sudo mkdir -p "$MOUNTDIR/tmp"
sudo mkdir -p "$MOUNTDIR/dev"
#sudo cp -a /dev/console "$MOUNTDIR/dev/"
#sudo cp -a /dev/ram0 "$MOUNTDIR/dev/"
#sudo cp -a /dev/null "$MOUNTDIR/dev/"
#sudo cp -a /dev/tty "$MOUNTDIR/dev/"
#sudo cp -a /dev/tty0 "$MOUNTDIR/dev/"
sudo mknod "$MOUNTDIR/dev/null" c 1 3
sudo mknod "$MOUNTDIR/dev/tty0" c 4 0
sudo mknod "$MOUNTDIR/dev/sda"  b 8 0
#sudo cp -a /dev/tty2 "$MOUNTDIR/dev/"
#sudo ln -s "$MOUNTDIR/bin/sh" "$MOUNTDIR/linuxrc"

echo "Unmounting..."
unmount

echo "Compressing..."
gzip -cvf "$FILE" > "$COMPRESSEDFILE"

echo "Done"
