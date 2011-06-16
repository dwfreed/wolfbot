# $Id: Makefile.in 84 2009-01-10 01:37:29Z chromerium $
CC = gcc -ggdb
CFLAGS = -Wall -DIN_BUILDING_LIBIRC -DENABLE_IPV6 -DENABLE_THREADS -DENABLE_DEBUG -D_REENTRANT
AR=ar cr
RANLIB=ranlib
INCLUDES=-I../include
DESTDIR=


OBJS = libircclient.o

all:	lib

lib:	libircclient.a

install: lib
	@echo "Installing libircclient"
	@if test ! -d $(DESTDIR)/include; then mkdir $(DESTDIR)/include; fi
	@if test ! -d $(DESTDIR)/lib; then mkdir $(DESTDIR)/lib; fi
	@cp ../include/libircclient.h $(DESTDIR)/include/libircclient.h
	@cp ../include/libirc_errors.h $(DESTDIR)/include/libirc_errors.h
	@cp ../include/libirc_events.h  $(DESTDIR)/include/libirc_events.h 
	@cp ../include/libirc_rfcnumeric.h  $(DESTDIR)/include/libirc_rfcnumeric.h 
	@cp ../include/libirc_options.h  $(DESTDIR)/include/libirc_options.h 
	@cp libircclient.a $(DESTDIR)/lib/libircclient.a

$(OBJS): utils.c dcc.c errors.c portable.c sockets.c colors.c

libircclient.a: $(OBJS)
	@echo "Building $@"
	@$(AR) libircclient.a $(OBJS)
	@$(RANLIB) libircclient.a

clean:
	@echo "Cleaning libircclient"
	@rm -f libircclient.a $(OBJS)

.c.o:
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

uninstall:
	@echo "Uninstalling libircclient"
	@rm -f $(DESTDIR)/include/libircclient.h
	@rm -f $(DESTDIR)/include/libirc_errors.h
	@rm -f $(DESTDIR)/include/libirc_events.h 
	@rm -f $(DESTDIR)/lib/libircclient.a

.PHONY: all lib install clean uninstall
