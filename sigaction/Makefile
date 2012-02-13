CC         = gcc
COPTS      = -Wall -g
LDLIBS     = pthread

execs      = sigaction

%.o: %.c ; $(CC) $(COPTS) -c $<
%  : %.o ; $(CC) $(COPTS) -o $@ $^ $(LDLIBS:%=-l%)

.PHONY: all clean

all: $(execs)
clean: ; rm -f $(execs) ./*.o

sigaction  : sigaction.o
sigaction.o: sigaction.c
