CC=gcc 

all : main.o interpreter.o shell.o shellmemory.o kernel.o ram.o cpu.o pcb.o memorymanager.o
	$(CC) -o mykernel main.o interpreter.o shell.o shellmemory.o kernel.o ram.o cpu.o pcb.o memorymanager.o 
	
kernel.o: kernel.c
	$(CC) -c kernel.c

cpu.o: cpu.c
	$(CC) -c cpu.c

pcb.o: pcb.c
	$(CC) -c pcb.c

ram.o: ram.c
	$(CC) -c ram.c

interpreter.o : interpreter.c 
	$(CC) -c interpreter.c 

shell.o : shell.c 
	$(CC) -c shell.c 

shellmemory.o : shellmemory.c
	$(CC) -c shellmemory.c 

memorymanager.o : memorymanager.c
	$(CC) -c memorymanager.c

main.o : main.c
	$(CC) -c main.c

.PHONY:
clean:
	rm -f interpreter.o shell.o shellmemory.o ram.o kernel.o cpu.o pcb.o memorymanager.o main.o

run:
	./mykernel

testfile:
	./mykernel < testfile.txt