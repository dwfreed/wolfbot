# $Id: Makefile.in 62 2005-03-09 21:11:53Z gyunaev $
CC = gcc
CFLAGS = -Wall -DIN_BUILDING_LIBIRC -O3 -DENABLE_THREADS -D_REENTRANT
AR=ar cr
RANLIB=ranlib
INCLUDES=-I../include
PREFIX = ${PWD}

OBJS = libircclient.o

all:	lib

lib:	libircclient.a

install: lib
	-mkdir ${PREFIX}/libircclient/include
	-mkdir ${PREFIX}/libircclient/lib
	cp ../include/libircclient.h ${PREFIX}/libircclient/include/libircclient.h
	cp ../include/libirc_errors.h ${PREFIX}/libircclient/include/libirc_errors.h
	cp ../include/libirc_events.h  ${PREFIX}/libircclient/include/libirc_events.h 
	cp ../include/libirc_options.h ${PREFIX}/libircclient/include/libirc_options.h
	cp ../include/libirc_rfcnumeric.h ${PREFIX}/libircclient/include/libirc_rfcnumeric.h

$(OBJS): utils.c dcc.c errors.c portable.c sockets.c colors.c

libircclient.a: $(OBJS)
	$(AR) libircclient.a $(OBJS)
	$(RANLIB) libircclient.a
	mv libircclient.a ${PREFIX}/libircclient/lib/libircclient.a

clean:
	rm -f ${PREFIX}/libircclient/lib/libircclient.a $(OBJS)

distclean: clean
	-rm -f Makefile

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<
