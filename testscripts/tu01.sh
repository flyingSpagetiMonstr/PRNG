#!/bin/bash
set -e  # Exit on any error

output="stored/TestU01.txt"

pushd ..
make clean
make lib
popd

gcc -std=c99 -Wall -O3 -o tu01 tu01.c \
    -Iinclude -Llib -ltestu01 -lprobdist -lmylib \
    -lm -lgmp \
    -I../include/  -L../bin -lprng

echo "Entering"
./tu01 > $output
echo "Fin!!!"

rm ./tu01

# shutdown -h +10  

