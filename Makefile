CC = gcc

all: clean foo

foo: cshell.o
	$(CC) cshell.c

clean: 
	rm -rf cshell.o
