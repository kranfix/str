CC=gcc -std=c11

all: pid.o planta.o sim.elf

sim.elf: main.c pid.o planta.o
	$(CC) -o $@ $^

pid.o: pid.c pid.h
	$(CC) -c $<

planta.o: planta.c planta.h
	$(CC) -c $<

clean:
	rm *.o
