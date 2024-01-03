cd ..
make rebuild

echo "running dieharder..."
./PRNG | dieharder -a -g 200 > sts-2.1.2/stored/dieharder.txt
# ./PRNG | dieharder -d 205 -g 200 > sts-2.1.2/stored/dieharder.txt
