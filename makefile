CC = gcc
OBJS = objs/PRNG.o objs/dump.o objs/bitstore.o
FLAGS = -I include/

.PHONY: run clean cleardump

run: PRNG.exe
	.\PRNG

clean:
	del /s *.o

cleardump: 
	del .\dumps\stream_len.dat
	del .\dumps\state.dat

# all: PRNG clean

PRNG.exe: $(OBJS) 
	$(CC) -o $@ $(OBJS)

objs/PRNG.o: PRNG.c include/PRNG.h include/dump.h include/bitstore.h
	$(CC) $(FLAGS) -c -o $@ $<

objs/dump.o: libs/dump.c include/dump.h include/PRNG.h 
	$(CC) $(FLAGS) -c -o $@ $<

objs/bitstore.o: libs/bitstore.c include/bitstore.h include/PRNG.h 
	$(CC) $(FLAGS) -c -o $@ $<