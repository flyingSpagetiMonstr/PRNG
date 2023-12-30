CC = gcc -O1
FLAGS = -I include/

OBJS = objs/dump.o objs/PRNG.o

MAIN = $(if $(shell echo $$OS), main.exe, main) # OS-specific 
RRM = $(if $(shell echo $$OS), del /s, rm -rf) # OS-specific command
RM = $(if $(shell echo $$OS), del, rm -f) # OS-specific command

.PHONY: clearobj cleardump 	\
	do run rerun 			\
	asm pre lib  


# =============================
do: $(MAIN) run 

rerun: clean do

run: 
	./main

# =============================
clean: clearobj
	$(RM) .\bin\libprng.a  
	$(RM) $(MAIN)

clearobj:
ifeq ($(OS), Windows_NT)  # Check for Windows
	$(RRM) *.o
else
	$(RRM) objs/*.o
endif
# =============================
lib: pre bin/libprng.a

pre: 
	@ if not exist bin mkdir bin
	@ if not exist objs mkdir objs

# =============================

cleardump: 
	$(RRM) .\dumps\stream_len.dat
	$(RRM) .\dumps\state.dat

asm: PRNG.asm

# =====================================================
objs/PRNG.o: PRNG.c include/PRNG-basics.h include/PRNG.h
	$(CC) $(FLAGS) -c -o $@ $<

objs/dump.o: libs/dump.c include/dump.h include/PRNG-basics.h 
	$(CC) $(FLAGS) -c -o $@ $<

PRNG.asm: PRNG.c $(HEADERS)
	$(CC) $(FLAGS) -S -o $@ $<

bin/libprng.a: $(OBJS)
	ar rcs $@ $(OBJS)
# =====================================================
$(MAIN): main.c include/PRNG.h bin/libprng.a 
	$(CC) $(FLAGS) -o $@ $< -L./bin -lprng
