void initialize_chip8(void);
void load_font(void);
void load_rom(void);	
void emulate_cycle(void);
void fetch_opcode(void);

unsigned short opcode;
unsigned char V[16];
unsigned short I;
unsigned short pc;
unsigned short stack[16];
unsigned short sp;
