$(CC)=gcc

sim.elf: main.c planta.o sys_ss.o pid.o
	$(CC) -o $@ $^ -lpthread

planta.o: planta.c planta.h
	$(CC) -c $<

sys_ss.o: sys_ss.c sys_ss.h
	$(CC) -c $<

pid.o: pid.c pid.h
	$(CC) -c $<

run: sim.elf
	./sim.elf

test.elf: test.c planta.o sys_ss.o pid.o
	$(CC) -o $@ $^

test: test.elf
	./test.elf

clean:
	rm -f *.o *.elf *.txt
