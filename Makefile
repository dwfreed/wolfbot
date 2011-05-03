# Name of the binary
PROG = wolfgame

# All of the C files we'll need go here, but with a .o extension
OBJS = main.o callbacks.o threaded_functions.o game.o

LIBS = glib-2.0 gthread-2.0 libcurl

CC = gcc -pthread -ggdb -Wall -Werror
CFLAGS = -I${PWD}/libircclient/include $(shell pkg-config --cflags ${LIBS})
LDFLAGS = -L${PWD}/libircclient/lib $(shell pkg-config --libs ${LIBS}) -lircclient

${PROG}: ${OBJS} libircclient/lib/libircclient.a Makefile
	${CC} -o ${PROG} ${OBJS} ${LDFLAGS}

%.o: %.c Makefile global.h
	${CC} ${CFLAGS} -c $< -o $@

run: ${PROG}
	./${PROG}

clean:
	rm -f ${PROG} ${OBJS}
	${MAKE} -C libircclient/src clean

libircclient/lib/libircclient.a: 
	${MAKE} -C libircclient/src PREFIX=${PWD}

global.h: callbacks.h threaded_functions.h game.h libircclient/lib/libircclient.a

.PHONY: run clean
