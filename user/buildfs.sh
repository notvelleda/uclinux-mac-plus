#!/bin/sh
set -e
cd "$(dirname "$0")"

FILE=root.img
SIZE_MB=${SIZE_MB:-32}
FSDIR=rootdir
OVERLAYDIR=rootdir-overlay
KERNEL_PATH=../uClinux/images/kernel
CMDLINE_FILE=cmdline
BOOTLOADER_DIR=../bootloader
STAGE=__stage

install_bootloader() {
    "$BOOTLOADER_DIR/installer" -b "$BOOTLOADER_DIR/boot_block.bin" -d "$BOOTLOADER_DIR/scsi_hdd_driver.bin" -s "$FILE"
}

for tool in mke2fs debugfs fakeroot od; do
    command -v "$tool" >/dev/null 2>&1 ||
    {
      echo "error: '$tool'";
      exit 1;
    }
done

# Initial verification
[ -f "$BOOTLOADER_DIR/boot_block.bin" ] ||
{
  echo "error: bootloader/boot_block.bin missing, run ./build.sh first";
  exit 1;
}
[ -d "$FSDIR/bin" ] ||
{
  echo "error: $FSDIR is empty, run ./build.sh first";
  exit 1;
}

echo "Building bootloader installer..."
make -C "$BOOTLOADER_DIR" installer

echo "Creating empty ${SIZE_MB} MB image and writing partition map + bootloader..."
rm -f "$FILE"
truncate -s "${SIZE_MB}M" "$FILE"
install_bootloader

# partition map entry lives in block 1 (offset 512), big endian:
#   +8  = first block of partition, +12 = number of blocks in partition
PART_START=$(od -An -tu4 --endian=big -j 520 -N4 "$FILE" | tr -d ' ')
PART_BLOCKS=$(od -An -tu4 --endian=big -j 524 -N4 "$FILE" | tr -d ' ')
OFFSET=$((PART_START * 512))
FS_KB=$((PART_BLOCKS * 512 / 1024))
echo "Partition starts at block $PART_START (byte offset $OFFSET), size ${FS_KB} KB"

echo "Preparing staging directory..."
rm -rf "$STAGE"
mkdir -p "$STAGE"
cp -a "$FSDIR"/. "$STAGE"/
cp -a "$OVERLAYDIR"/. "$STAGE"/
mkdir -p "$STAGE/dev" "$STAGE/var/tmp"
ln -sfn /var/tmp "$STAGE/tmp"

if [ -f "$KERNEL_PATH" ]; then
    cp "$KERNEL_PATH" "$STAGE/kernel"
else
    echo "warning: kernel hasn't been built yet (uClinux/build.sh), resulting image will not be bootable"
fi

[ -f "$CMDLINE_FILE" ] || cp "${CMDLINE_FILE}.default" "$CMDLINE_FILE"
cp "$CMDLINE_FILE" "$STAGE/cmdline"

echo "Building ext2 filesystem inside the image..."
# fakeroot makes everything look like it belongs to root (uid 0) without being root
fakeroot sh -c "chown -R 0:0 '$STAGE' && mke2fs -q -F -t ext2 -O none -I 128 -b 1024 -E offset=$OFFSET,root_owner=0:0 -d '$STAGE' '$FILE' ${FS_KB}"

echo "Creating device nodes..."
CMDS=$(mktemp)
{
    # debugfs's mknod doesn't understand paths, so go into /dev first
    echo "cd /dev"
    echo "mknod mem c 1 1"
    echo "mknod kmem c 1 2"
    echo "mknod null c 1 3"
    echo "mknod port c 1 4"
    echo "mknod zero c 1 5"
    echo "mknod full c 1 7"
    echo "mknod random c 1 8"
    echo "mknod urandom c 1 9"
    for i in 0 1 2 3 4 5 6 7 8 9 a b c d e f; do
        n=$(printf '%d' "0x$i")
        echo "mknod ptyp$i c 2 $n"
        echo "mknod ttyp$i c 3 $n"
    done
    echo "mknod tty0 c 4 0"
    echo "mknod tty c 5 0"
    echo "mknod console c 5 1"
    echo "mknod ttyS0 c 4 64"
    echo "mknod ttyS1 c 4 65"
    echo "mknod cua0 c 5 64"
    echo "mknod cua1 c 5 65"
    echo "mknod sound c 14 0"
    echo "mknod fd0 b 2 0"
    echo "mknod fd1 b 2 1"
    echo "mknod fb0 c 29 0"
    echo "mknod fb0autodetect c 29 1"
    for i in 0 1 2 3 4 5 6 7; do
        echo "mknod fb0user$i c 29 $((24 + i))"
    done
    # SCSI disks: sda..sdg, 16 minors each
    base=0
    for d in a b c d e f g; do
        echo "mknod sd$d b 8 $base"
        for i in $(seq 15); do
            echo "mknod sd$d$i b 8 $((base + i))"
        done
        base=$((base + 16))
    done
} > "$CMDS"

# debugfs creates nodes with mode 0000: give each one normal permissions (rw-rw-rw- / rw-rw----)
awk '/^mknod/ { print; print "sif " $2 " mode " (($3 == "c") ? "020666" : "060660"); next } { print }' "$CMDS" > "$CMDS.2"
mv "$CMDS.2" "$CMDS"
debugfs -w -f "$CMDS" "${FILE}?offset=${OFFSET}" > /dev/null
rm -f "$CMDS"

echo "Installing bootloader again (to be sure the boot block is intact)..."
install_bootloader

rm -rf "$STAGE"

echo "Checking filesystem..."
e2fsck -fn "${FILE}?offset=${OFFSET}" || true

echo "Done: $(pwd)/$FILE ($(du -h "$FILE" | cut -f1) on disk, ${SIZE_MB} MB apparent)"
