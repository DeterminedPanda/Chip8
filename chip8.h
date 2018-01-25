void initialize_chip8(void);
void load_font(void);
void load_rom(void);	
void emulate_cycle(void);
void fetch_opcode(void);

extern unsigned short opcode;
extern unsigned char V[16];
extern unsigned short I;
extern unsigned short pc;
extern unsigned short stack[16];
extern unsigned short sp;
