.PHONY: all
all: c.a

c.a: parse_c.o keywords_c.o
	ar rT $@ $?

parse_c.o: parse_c.c keywords_c.h keywords_c.h
keywords_c.o: keywords_c.c keywords_c.h
