#!/bin/bash
# This shell script compile the .c source file to corresponding
# .so object file
# Usage:
#   MakeClib.sh test.c

INPUT=$1
OUTPUT=${INPUT%.*}.so

gcc -shared -fPIC -o ${OUTPUT} ${INPUT}
