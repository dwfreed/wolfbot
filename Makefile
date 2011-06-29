# Name of the binary
PROG = wolfbot

# All of the C files we'll need go here, but with a .o extension
OBJS = callbacks.o threaded_functions.o game.o util.o conf.o main.o

LIBS = glib-2.0 gthread-2.0 libconfig

CC = gcc -ggdb -Wall -Werror
CFLAGS = -I${CURDIR}/libircclient -DWOLFBOT_VERSION='"$(shell hg id -n)"' $(shell pkg-config --cflags ${LIBS})
LDFLAGS = -Wl,-rpath,${CURDIR}/libircclient -L${CURDIR}/libircclient $(shell pkg-config --libs ${LIBS}) -lircclient -lm -ldl -rdynamic

all: ${PROG} auth

auth: $(patsubst %.c,%.so,$(wildcard auth/*/auth.c auth/*/*/auth.c))

${PROG}: ${OBJS}
	@echo "Building $@"
	@${CC} -o $@ ${OBJS} ${LDFLAGS}

global.h: libircclient callbacks.h threaded_functions.h game.h util.h conf.h auth.h

libircclient:
	@${MAKE} -C libircclient DEBUG=1

%.o: %.c Makefile global.h
	@echo "Compiling $<"
	@${CC} ${CFLAGS} -c $< -o $@

%.i: %.c Makefile global.h
	@echo "Prepocessing $<"
	@${CC} ${CFLAGS} -E $< -o $@

%.so: %.c Makefile global.h
	@echo "Compiling $<"
	@${CC} -I${CURDIR} ${CFLAGS} -shared -nostartfiles -nostdlib -fPIC $< -o $@

run: ${PROG}
	@echo "Running ${PROG}"
	@./${PROG}

clean:
	@echo "Cleaning ${PROG}"
	@rm -f ${PROG} ${OBJS} $(wildcard auth/*/auth.so auth/*/*/auth.so)
	@${MAKE} -C libircclient clean

leakcheck: ${PROG}
	@echo "Leak-checking ${PROG}"
	@G_SLICE="always-malloc" valgrind --leak-check=full --track-fds=yes --show-reachable=yes --track-origins=yes ./${PROG}

.PHONY: all auth libircclient run clean leakcheck
