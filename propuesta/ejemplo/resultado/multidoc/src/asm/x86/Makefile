.PHONY: all
all: x86.a

x86.a: parse_x86.o opcodes_x86.o
	ar rT $@ $?

parse_x86.o: parse_x86.c parse_x86.h opcodes_x86.h
opcodes_x86.o: opcodes_x86.c opcodes_x86.h
