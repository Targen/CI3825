#!/usr/bin/env bash

size=20

for ((i=1; i <= size; ++i))
do
	for ((j=1; j < size; ++j))
	do
		cat > "d0x${i}x${j}.h" <<-EOF
			#ifndef D0X${i}X${j}_H
			#define D0X${i}X${j}_H
			
			#include "d0x${i}x$((j + 1)).h"
			
			typedef t0x${i}x$((j + 1)) t0x${i}x${j};
			
			t0x${i}x${j} d0x${i}x${j}();
			
			#endif
		EOF

		cat > "d0x${i}x${j}.c" <<-EOF
			#include "d0x${i}x${j}.h"
			#include "d0x${i}x$((j + 1)).h"
			
			t0x${i}x${j} d0x${i}x${j}() {
			        return d0x${i}x$((j + 1))();
			}
		EOF
	done

	cat > "d0x${i}x20.h" <<-EOF
		#ifndef D0X${i}X20_H
		#define D0X${i}X20_H
		
		typedef int t0x${i}x20;
		t0x${i}x20 d0x${i}x20();
		
		#endif
	EOF

	cat > "d0x${i}x20.c" <<-EOF
		#include "d0x${i}x20.h"
		
		t0x${i}x20 d0x${i}x20() {
		        return ${i};
		}
	EOF
done



cat > "main.c" <<-EOF
	#include <stdio.h>
	
EOF

for ((i=1; i <= size; ++i))
do
	cat >> "main.c" <<-EOF
		#include "d0x${i}x1.h"
	EOF
done

cat >> "main.c" <<-EOF
	
	int main() {
EOF

for ((i=1; i <= size; ++i))
do
	cat >> "main.c" <<-EOF
		        printf("%d\n", d0x${i}x1());
	EOF
done

cat >> "main.c" <<-EOF
	}
EOF
