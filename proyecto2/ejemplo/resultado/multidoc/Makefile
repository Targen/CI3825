.PHONY: all force
all: multidoc

multidoc: src/src.a lib/lib.a main.o args.o
	$(CC) $(CPPFLAGS) $(CCFLAGS) -o $@ $^

src/src.a: force
	$(MAKE) -C src

lib/lib.a: force
	$(MAKE) -C lib

main.o: main.c main.h args.h
args.o: args.c args.h
