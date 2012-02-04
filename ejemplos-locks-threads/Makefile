CC         = gcc
COPTS      = -Wall -g
LDLIBS     = pthread

execs      = printer prodcons

build-exec = $(CC) $(COPTS) -o $@ $^ $(LDLIBS:%=-l%)
build-obj  = $(CC) $(COPTS) -c $<

.PHONY: all clean

all: $(execs)
clean: ; rm -f $(execs) ./*.o

prodcons  : prodcons.o ; $(build-exec)
prodcons.o: prodcons.c ; $(build-obj)
printer   : printer.o  ; $(build-exec)
printer.o : printer.c  ; $(build-obj)
