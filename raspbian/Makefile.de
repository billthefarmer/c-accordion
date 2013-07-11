################################################################################
#
#  Makefile for Accordion
#
################################################################################

GCC	= gcc
CFLAGS  = -O2 -std=gnu99 -L. -L/usr/local/lib -Wl,-R. -Wl,-R/usr/local/lib
LIBS    = -lmidi -lsonivox
all:	Accordion

clean:
	rm Accordion

%:	%.c
	$(GCC) $< -o $@ $(CFLAGS) $(LIBS) `pkg-config --cflags --libs gtk+-2.0`
