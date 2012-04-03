#!/usr/bin/env bash

set -e

size=100
path="$(ruby -e "puts [*1..$size].map { |n| 'subdir' + n.to_s }.join '/'")"



mkdir -p "$path"
(
	cd "$path"

	cat > 'indirect.h' <<-EOF
		#ifndef INDIRECT_H
		#define INDIRECT_H
		
		int indirect();
		
		#endif
	EOF

	cat > 'indirect.c' <<-EOF
		#include "indirect.h"
		
		int indirect() {
		        return 42;
		}
	EOF
)



cat > 'main.c' <<-EOF
	#include <stdio.h>
	
	#include "$path/indirect.h"
	
	int main() {
	        printf("%d\n", indirect());
	        return 0;
	}
EOF



cat > 'Makefile.ref' <<EOF
.PHONY: all clean

all: indirect

clean:
	rm -f indirect main.o $path/indirect.o

indirect: main.o $path/indirect.o
	gcc -o \$@ \$^

EOF



shopt -s globstar
gcc -MM ./**/*.c >> 'Makefile.ref'
