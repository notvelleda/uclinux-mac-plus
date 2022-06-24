#
# Makefile for Linux arch/m68k/console source directory
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#

L_TARGET = console.a
# replaced 68328fb.o with macfb.o, don't want unused code!
L_OBJS = macfb.o fbcon.o fonts.o font_8x16.o font_8x8.o pearl_8x8.o mini_4x6.o terminus_6x12.o
M_OBJS =

include $(TOPDIR)/Rules.make
