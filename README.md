# uClinux for the Mac Plus
(specifically ones with 4 mb of ram since i was too lazy to write a decent bootloader)

## requirements
you need Think C 5.0 to build the stage 1 loader, however the disk image includes a pre-built version for convenience

you'll also need [Mini vMac](https://www.gryphel.com/c/minivmac/index.html) and [ImportFl](https://www.gryphel.com/c/minivmac/extras/importfl/) to import files into Mini vMac, or some other way to put things in a disk image with another emulator

for everything else, you need Docker and a POSIX compatible shell

the Docker build environment is created by running build.sh in the buildenv directory

## building things
run build.sh in the stage2, uClinux, and user directories to compile the stage2 loader, kernel, and user mode programs, respectively

run buildfs.sh in the user directory to generate a root filesystem image (64 mb by default, can be changed by changing the SIZE variable in the file)

## running things
you'll want to have the stage 1 bootloader application, uClinux/images/kernel.img, (optionally, uClinux/images/initrd.img, but again i wouldn't recommend it), and stage2/stage2.bin all in the same directory in a Mac volume

if you don't have any scsi storage that you can put the root fs on, you'll want to have an initrd (but it takes ages to decompress and doesn't like uncompressed images). otherwise, you'll want to store your root fs in an ext2 formatted (no extra features, 128 bit inodes, `mkfs.ext2 -O none -I 128`) partition or entire scsi device (i've only tested using an entire scsi device on my bluescsi)

when running the bootloader, if your root filesystem is on a scsi device, you'll put `root=/dev/sdX` in the kernel command line textbox when running the bootloader (where `sdX` can be replace with `sda`, `sdb`, `sdc`, etc. depending on the scsi device number of the root fs. partitions are the same as normal). it saves the command line, so if it works you won't have to remember what to type in every time

## what works (from what i remember)
- framebuffer
- keyboard
- scsi controller

## what doesn't
basically everything else. namely the computer locks up if you move the mouse
