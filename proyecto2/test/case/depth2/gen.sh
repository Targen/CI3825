#!/usr/bin/env bash

size=20

for ((i=1; i <= size; ++i))
do
	(mkdir -p "subdir$i/sub/" && cd "subdir$i/sub/" && {
		for ((j=1; j < size; ++j))
		do
			cat > "d2x${i}x${j}.h" <<-EOF
				#ifndef D2X${i}X${j}_H
				#define D2X${i}X${j}_H
				
				#include "d2x${i}x$((j + 1)).h"
				
				typedef t2x${i}x$((j + 1)) t2x${i}x${j};
				
				t2x${i}x${j} d2x${i}x${j}();
				
				#endif
			EOF

			cat > "d2x${i}x${j}.c" <<-EOF
				#include "d2x${i}x${j}.h"
				#include "d2x${i}x$((j + 1)).h"
				
				t2x${i}x${j} d2x${i}x${j}() {
					return d2x${i}x$((j + 1))();
				}
			EOF
		done

		cat > "d2x${i}x20.h" <<-EOF
			#ifndef D2X${i}X20_H
			#define D2X${i}X20_H
			
			typedef int t2x${i}x20;
			t2x${i}x20 d2x${i}x20();
			
			#endif
		EOF

		cat > "d2x${i}x20.c" <<-EOF
			#include "d2x${i}x20.h"
			
			t2x${i}x20 d2x${i}x20() {
				return ${i};
			}
		EOF
	}) || exit 1
done



cat > "main.c" <<-EOF
	#include <stdio.h>
	
EOF

for ((i=1; i <= size; ++i))
do
	cat >> "main.c" <<-EOF
		#include "subdir$i/sub/d2x${i}x1.h"
	EOF
done

cat >> "main.c" <<-EOF
	
	int main() {
EOF

for ((i=1; i <= size; ++i))
do
	cat >> "main.c" <<-EOF
		        printf("%d\n", d2x${i}x1());
	EOF
done

cat >> "main.c" <<-EOF
	}
EOF
