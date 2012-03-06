.PHONY: all
all: mips.a

mips.a: parse_mips.o opcodes_mips.o
	ar rT $@ $?

parse_mips.o: parse_mips.c parse_mips.h opcodes_mips.h
opcodes_mips.o: opcodes_mips.c opcodes_mips.h
