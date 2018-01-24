void initialize_chip8();
void load_rom(void);	
void emulate_cycle(void);
void fetch_opcode(void);

extern unsigned short opcode;
