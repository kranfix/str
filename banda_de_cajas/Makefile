$(CC)=gcc

sim.elf: main.c planta.o
	$(CC) -o $@ $^ -lpthread

planta.o: planta.c planta.h
	$(CC) -c $<

run: sim.elf
	./sim.elf

test.elf: test.c planta.o
	$(CC) -o $@ $^

test: test.elf
	./test.elf

clean:
	rm -f *.o *.elf *.txt
