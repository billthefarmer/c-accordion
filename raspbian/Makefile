################################################################################
#
#  Makefile for Accordion
#
################################################################################

GCC	= gcc
CFLAGS  = -O2 -fPIC -std=gnu99 -L/usr/local/lib -Wl,-R/usr/local/lib \
	-Wl,-z,relro
LIBS    = -lmidi -lsonivox -lX11
all:	Accordion

clean:
	rm Accordion

%:	%.c
	$(GCC) $< -o $@ $(CFLAGS) $(LIBS) `pkg-config --cflags --libs gtk+-2.0`
