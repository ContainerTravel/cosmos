# !/bin/bash
echo "Start......"
echo "Cosmos all code line:"
find  . -name "*.inc" -o -name "*.asm" -o -name "*.S" -o -name "*.h" -o -name "*.c" | xargs cat|grep -v ^$| wc -l | sort -n
echo "Cosmos ASM code line:"
find  . -name "*.S" -o -name "*.asm" | xargs cat|grep -v ^$| wc -l | sort -n
echo "Cosmos C code line:"
find  . -name "*.c" | xargs cat|grep -v ^$| wc -l | sort -n
echo "Cosmos H code line:"
find  . -name "*.h" -o -name "*.inc" | xargs cat|grep -v ^$| wc -l | sort -n
echo "End!"