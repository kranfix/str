CC=gcc -std=c11

all: pid.o planta.o

pid.o: pid.c pid.h
	$(CC) -c $<

planta.o: planta.c planta.h
	$(CC) -c $<

clean:
	rm *.o
