
output: chip8.o display.o main.o opcode.o
	gcc src/main.c src/chip8.c src/opcode.c src/display.c -lSDL2 -o emulator 

main.o: src/main.c
	gcc -c src/main.c

chip8.o: src/chip8.c
	gcc -c src/chip8.c

opcode.o: src/opcode.c
	gcc -c src/opcode.c

display.o: src/display.c
	gcc -c src/display.c

clean:
	rm *.o emulator 
