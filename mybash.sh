#!/bin/bash
gcc -c main.c shell.c interpreter.c shellmemory.c kernel.c cpu.c pcb.c ram.c memorymanager.c
gcc -o mykernel main.o shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o memorymanager.o
#Clear all of the object files
rm -f shell.o interpreter.o shellmemory.o kernel.o cpu.o pcb.o ram.o main.o memorymanager.o