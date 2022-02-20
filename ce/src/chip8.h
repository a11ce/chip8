#pragma once

#include <stdint.h>

typedef struct chip8 *chip8_t;

struct chip8 {
  uint8_t memory[4096];
  uint8_t display[2048];

  uint16_t pc;
  uint16_t i;

  uint16_t stack[16];
  uint8_t sp;

  uint8_t delay;
  uint8_t sound;

  uint8_t regs[16];

  uint8_t keypad[16];
};

chip8_t initChip8();
void loadRom(chip8_t console, const char *filePath);
void cycle(chip8_t console);

void printRegs(chip8_t console);
void printMem(chip8_t console);
