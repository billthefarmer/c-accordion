################################################################################
#
#  Makefile for Accordion
#
################################################################################

GCC	= gcc
WINDRES = windres

all:	Accordion.exe

Accordion.exe:	Accordion.c Accordion.o

Accordion.o:	Accordion.rc Accordion.manifest Accordion.ico

clean:
	rm *.exe

%.exe:	%.c
	$(GCC) -o $* $< $*.o -std=gnu99 -mwindows -lcomctl32 -lwinmm

%.o:	%.rc
	$(WINDRES) -o $@ $<
