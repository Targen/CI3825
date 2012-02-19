CC    := gcc
COPTS := -Wall -O3

subdirs := foo bar
subarcs := $(subdirs:%=%/arc.a)

srcs := $(wildcard *.c)
objs := $(srcs:%.c=%.o)

.PHONY: all clean $(subdirs) force

all: foobar arc.a($(subarcs))

clean:
	find .             -type f -name '*.o' -o -name '*.a' -exec rm -f ./{} +
	find . -mindepth 2 -type f -name Makefile             -exec rm -f ./{} +
	rm -f foobar $(objs)

$(subarcs): force
	$(MAKE) -C $(@:%/arc.a=%)

foobar: $(objs) $(subarcs)
	gcc -o foobar $(objs) $(subarcs)

main.o: main.c foo/foo.h bar/bar.h f.h
f.o: f.c f.h
