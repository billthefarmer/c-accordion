################################################################################
#
#  Makefile for Accordion
#
################################################################################

GCC	= gcc
WINDRES = windres

all:	Accordion.exe

Accordion.exe:	Accordion.c Accordion.o

clean:
	rm *.exe

%.exe:	%.c
	$(GCC) -o $* $< $*.o -mwindows -lcomctl32 -lwinmm

%.o:	%.rc
	$(WINDRES) -o $@ $<
