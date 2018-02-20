#define MEMORY_SIZE 4096 //total memory size
#define ROM_SP 0x200 //the rom starting point in memory

struct Chip8 {
	unsigned char memory[MEMORY_SIZE];
	unsigned short pc;
	unsigned short opcode;
	unsigned char V[16];
	unsigned short I;
	unsigned short stack[16];
	unsigned short sp;
	unsigned char delay_timer;
	unsigned char sound_timer;
	unsigned char gfx[64 * 32];
	unsigned int draw_flag;
	unsigned char keys[16];
};

void start_emulation(void);
void initialize_chip(struct Chip8 *chip);
void load_font(struct Chip8 *chip);
void load_rom(struct Chip8 *chip);	
void emulate_cycle(struct Chip8 *chip);
void fetch_opcode(struct Chip8 *chip);
