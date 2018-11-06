CC=gcc
CFLAGS=-lWarn -pedantic

lab06: lab06.o libmyifttt.a
	cc lab06.o -L. -lmyifttt -lcurl -lwiringPi -o lab06

libmyifttt.a:	ifttt.o
	ar -rcs libmyifttt.a ifttt.o

ifttt.o: 	ifttt.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

lab06.o:	lab06.c ifttt.h
	$(CC) $(CFLAGS) -c -ansi $<

all:	tester

clean:
	rm lab06 *.o