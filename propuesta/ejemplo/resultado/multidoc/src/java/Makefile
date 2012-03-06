.PHONY: all
all: java.a

java.a: parse_java.o keywords_java.o
	ar rT $@ $?

parse_java.o: parse_java.c keywords_java.h keywords_java.h
keywords_java.o: keywords_java.c keywords_java.h
