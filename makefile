CC = gcc -O3
FLAGS = -I include/

OBJS = objs/dump.o objs/PRNG.o
LIBNAME = mini

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
	$(RM) .\bin\lib$(LIBNAME).a  
	$(RM) $(MAIN)

clearobj:
ifeq ($(OS), Windows_NT)  # Check for Windows
	$(RRM) *.o
else
	$(RRM) objs/*.o
endif
# =============================
lib: pre bin/lib$(LIBNAME).a

pre: 
ifeq ($(OS), Windows_NT)  # Check for Windows
	@ if not exist bin mkdir bin
	@ if not exist objs mkdir objs
else
	@ mkdir -p bin
	@ mkdir -p objs
endif
# =============================

cleardump: 
	$(RRM) .\dumps\stream_len.dat
	$(RRM) .\dumps\state.dat

asm: PRNG.asm

# =====================================================
objs/PRNG.o: PRNG.c include/PRNG.h include/components.h include/dump.h 
	$(CC) $(FLAGS) -c -o $@ $<

objs/dump.o: libs/dump.c include/dump.h
	$(CC) $(FLAGS) -c -o $@ $<

PRNG.asm: PRNG.c $(HEADERS)
	$(CC) $(FLAGS) -S -o $@ $<

bin/lib$(LIBNAME).a: $(OBJS)
	ar rcs $@ $(OBJS)
# =====================================================
$(MAIN): main.c include/PRNG.h bin/lib$(LIBNAME).a
	$(CC) $(FLAGS) -o $@ $< -L./bin -l$(LIBNAME)
