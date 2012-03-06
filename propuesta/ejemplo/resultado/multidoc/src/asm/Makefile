.PHONY: all force
all: asm.a

asm.a: x86/x86.a mips/mips.a asm.o marmota.o
	ar rT $@ $?

x86/x86.a: force
	$(MAKE) -C x86

mips/mips.a: force
	$(MAKE) -C mips

asm.o: asm.c asm.h marmota.h archs.h
marmota.o: marmota.c marmota.h archs.h
