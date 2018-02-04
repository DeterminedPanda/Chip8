void initialize_chip8(void);
void load_font(void);
void load_rom(void);	
void emulate_cycle(void);
void fetch_opcode(void);

unsigned char memory[4096];
unsigned char chip8_fontset[80];
unsigned short opcode;
unsigned char V[16];
unsigned short I;
unsigned short pc;
unsigned short stack[16];
unsigned short sp;
unsigned char delay_timer;
unsigned char sound_timer;
unsigned char gfx[64 * 32];
unsigned int draw_flag;
unsigned char key[16];
