CC=gcc
CFLAGS=-I.
DEPS = linkedList.h
OBJ = lab1.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lab1: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)