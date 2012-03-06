.PHONY: all
all: cc.a

cc.a: parse_cc.o keywords_cc.o
	ar rT $@ $?

parse_cc.o: parse_cc.c keywords_cc.h keywords_cc.h
keywords_cc.o: keywords_cc.c keywords_cc.h
