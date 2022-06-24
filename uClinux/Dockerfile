# syntax=docker/dockerfile:1
#FROM i686/ubuntu
FROM uclinux-buildenv:0.1
WORKDIR /linux/
COPY . .
RUN make clean
RUN make dep
RUN make
