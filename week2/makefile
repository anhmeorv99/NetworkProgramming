CC=gcc
CFLAGS=-I.
OBJ = check_domain.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

check_domain: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
