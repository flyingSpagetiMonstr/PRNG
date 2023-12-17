CC = gcc
OBJS = objs/PRNG.o objs/bitstore.o objs/dump.o
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

objs/PRNG.o: PRNG.c include/PRNG.h include/bitstore.h include/dump.h
	$(CC) $(FLAGS) -c -o $@ $<

objs/bitstore.o: libs/bitstore.c include/bitstore.h
	$(CC) $(FLAGS) -c -o $@ $<

objs/dump.o: libs/dump.c include/dump.h
	$(CC) $(FLAGS) -c -o $@ $<