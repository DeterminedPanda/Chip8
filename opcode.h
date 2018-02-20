#include "chip8.h"

void (*chip8_table[16])();

void cpu_0(struct Chip8 *chip);
void cpu_1(struct Chip8 *chip);
void cpu_2(struct Chip8 *chip);
void cpu_3(struct Chip8 *chip);
void cpu_4(struct Chip8 *chip);
void cpu_5(struct Chip8 *chip);
void cpu_6(struct Chip8 *chip);
void cpu_7(struct Chip8 *chip);
void cpu_8(struct Chip8 *chip);
void cpu_9(struct Chip8 *chip);
void cpu_a(struct Chip8 *chip);
void cpu_b(struct Chip8 *chip);
void cpu_c(struct Chip8 *chip);
void cpu_d(struct Chip8 *chip);
void cpu_e(struct Chip8 *chip);
void cpu_f(struct Chip8 *chip);
