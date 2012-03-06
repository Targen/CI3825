.PHONY: all
all: lib.a

lib.a: libdoc.o
	ar rT $@ $?

libdoc.o: libdoc.c libdoc.h
