CC=gcc
CCFLAGS= -g -std=c99 -Wall -pedantic

# % matches all
# %< is the source file (.c file)

%.o: %.c
	$(CC) -c $(CCFLAGS) $<

app: app.o functions.o 
	$(CC) -o app app.o functions.o 

# test: test.o
# 	gcc test.o -o  test
# test.o: test.c bmp.h
# 	gcc -c -Wall -pedantic -g test.c

clean: 
	del *.o	output