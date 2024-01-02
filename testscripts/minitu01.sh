#!/bin/bash
set -e  # Exit on any error

output="stored/TestU01-min.txt"

pushd ..
make clean
make lib
popd

gcc -std=c99 -Wall -O3 -o minitu01 minitu01.c \
    -Iinclude -Llib -ltestu01 -lprobdist -lmylib \
    -lm -lgmp \
    -I../include/ -L../bin -lmini

echo "Entering minitu01"
./minitu01 > $output
echo "Fin!!!"

rm ./minitu01

# shutdown -h +10  

