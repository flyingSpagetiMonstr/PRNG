CC = gcc -O1
OBJS = objs/PRNG.o objs/dump.o
FLAGS = -I include/
TARGET = PRNG.exe


.PHONY: run clearobj cleardump asm build rebuild

run: $(TARGET)
	.\PRNG

build: $(TARGET)

rebuild: clean build

clearobj:
	del /s *.o

clean: 
	del $(TARGET)

cleardump: 
	del .\dumps\stream_len.dat
	del .\dumps\state.dat

asm: PRNG.asm


PRNG.exe: $(OBJS) 
	$(CC) -o $@ $(OBJS)

objs/PRNG.o: PRNG.c include/PRNG.h include/dump.h
	$(CC) $(FLAGS) -c -o $@ $<

objs/dump.o: libs/dump.c include/dump.h include/PRNG.h 
	$(CC) $(FLAGS) -c -o $@ $<

PRNG.asm: PRNG.c include/PRNG.h include/dump.h 
	$(CC) $(FLAGS) -S -o $@ $<