.PHONY: all force
all: src.a

src.a: c/c.a cc/cc.a asm/asm.a java/java.a multidoc.o xml.o
	ar rT $@ $?

c/c.a: force
	$(MAKE) -C c

cc/cc.a: force
	$(MAKE) -C cc

asm/asm.a: force
	$(MAKE) -C asm

java/java.a: force
	$(MAKE) -C java

multidoc.o: multidoc.c multidoc.h xml.h asm/archs.h
xml.o: xml.c xml.h asm/archs.h
