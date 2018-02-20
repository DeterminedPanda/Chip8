
output: chip8.o display.o main.o opcode.o
	gcc main.c chip8.c opcode.c display.c -lSDL2 -o emulator 

main.o: main.c
	gcc -c main.c

chip8.o: chip8.c
	gcc -c chip8.c

opcode.o: opcode.c
	gcc -c opcode.c

display.o: display.c
	gcc -c display.c

clean:
	rm *.o emulator 
