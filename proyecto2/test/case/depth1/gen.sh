#!/usr/bin/env bash

size=20

for ((i=1; i <= size; ++i))
do
	(mkdir -p "subdir$i" && cd "subdir$i" && {
		for ((j=1; j < size; ++j))
		do
			cat > "d1x${i}x${j}.h" <<-EOF
				#ifndef D1X${i}X${j}_H
				#define D1X${i}X${j}_H
				
				#include "d1x${i}x$((j + 1)).h"
				
				typedef t1x${i}x$((j + 1)) t1x${i}x${j};
				
				t1x${i}x${j} d1x${i}x${j}();
				
				#endif
			EOF

			cat > "d1x${i}x${j}.c" <<-EOF
				#include "d1x${i}x${j}.h"
				#include "d1x${i}x$((j + 1)).h"
				
				t1x${i}x${j} d1x${i}x${j}() {
				        return d1x${i}x$((j + 1))();
				}
			EOF
		done

		cat > "d1x${i}x20.h" <<-EOF
			#ifndef D1X${i}X20_H
			#define D1X${i}X20_H
			
			typedef int t1x${i}x20;
			t1x${i}x20 d1x${i}x20();
			
			#endif
		EOF

		cat > "d1x${i}x20.c" <<-EOF
			#include "d1x${i}x20.h"
			
			t1x${i}x20 d1x${i}x20() {
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
		#include "subdir$i/d1x${i}x1.h"
	EOF
done

cat >> "main.c" <<-EOF
	
	int main() {
EOF

for ((i=1; i <= size; ++i))
do
	cat >> "main.c" <<-EOF
		        printf("%d\n", d1x${i}x1());
	EOF
done

cat >> "main.c" <<-EOF
	}
EOF
