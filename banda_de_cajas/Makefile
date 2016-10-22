$(CC)=gcc

sim.elf: planta.o

planta.o: planta.c planta.h
	$(CC) -c $<

clean:
	rm -f *.o *.elf
