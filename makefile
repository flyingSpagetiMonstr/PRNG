CC = gcc -O1
FLAGS = -I include/

OBJS = objs/PRNG.o objs/dump.o
TARGET = $(if $(shell echo $$OS), PRNG.exe, PRNG) # OS-specific 

RM = $(if $(shell echo $$OS), del /s, rm -rf) # OS-specific command

.PHONY: run clearobj cleardump asm build rebuild re

all: rebuild run 

run: 
	./PRNG

build: $(TARGET)

rebuild: clean build

clean: clearobj
	$(RM) $(TARGET)

clearobj:
ifeq ($(OS), Windows_NT)  # Check for Windows
	$(RM) *.o
else
	$(RM) objs/*.o
endif

cleardump: 
	$(RM) .\dumps\stream_len.dat
	$(RM) .\dumps\state.dat

asm: PRNG.asm

$(TARGET): $(OBJS) 
	$(CC) -o $@ $(OBJS)

objs/PRNG.o: PRNG.c include/PRNG.h include/dump.h
	$(CC) $(FLAGS) -c -o $@ $<

objs/dump.o: libs/dump.c include/dump.h include/PRNG.h 
	$(CC) $(FLAGS) -c -o $@ $<

PRNG.asm: PRNG.c include/PRNG.h include/dump.h 
	$(CC) $(FLAGS) -S -o $@ $<