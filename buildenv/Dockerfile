# syntax=docker/dockerfile:1
FROM i386/ubuntu
WORKDIR /
RUN apt-get -y update
RUN apt-get -y install make libncurses5-dev binutils gcc
COPY m68k-elf-tools-20030314.sh .
RUN tail -n +43 m68k-elf-tools-20030314.sh | gunzip > tools.tar
RUN tar xvf tools.tar
