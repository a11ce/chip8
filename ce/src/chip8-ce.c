#include "chip8.h"
#include <stdlib.h>
#include <string.h>
#include <tice.h>

/*
see
http://www.cs.columbia.edu/~sedwards/classes/2016/4840-spring/designs/Chip8.pdf
*/
#define X (opCode & 0x0F00) >> 8
#define Y (opCode & 0x00F0) >> 4
#define K (opCode & 0x00FF)
#define N (opCode & 0x0FFF)

static void dxyn(chip8_t console, uint16_t opCode);

uint8_t font[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
    0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
    0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
    0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
    0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
    0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
    0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
    0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
    0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
    0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
    0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
    0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
    0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
    0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
    0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
    0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
};

uint8_t rom_ibm[132] = {
    0x00, 0xE0, 0xA2, 0x2A, 0x60, 0x0C, 0x61, 0x08, 0xD0, 0x1F, 0x70, 0x09,
    0xA2, 0x39, 0xD0, 0x1F, 0xA2, 0x48, 0x70, 0x08, 0xD0, 0x1F, 0x70, 0x04,
    0xA2, 0x57, 0xD0, 0x1F, 0x70, 0x08, 0xA2, 0x66, 0xD0, 0x1F, 0x70, 0x08,
    0xA2, 0x75, 0xD0, 0x1F, 0x12, 0x28, 0xFF, 0x00, 0xFF, 0x00, 0x3C, 0x00,
    0x3C, 0x00, 0x3C, 0x00, 0x3C, 0x00, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF,
    0x00, 0x38, 0x00, 0x3F, 0x00, 0x3F, 0x00, 0x38, 0x00, 0xFF, 0x00, 0xFF,
    0x80, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0x80, 0x00, 0x80, 0x00, 0xE0, 0x00,
    0xE0, 0x00, 0x80, 0xF8, 0x00, 0xFC, 0x00, 0x3E, 0x00, 0x3F, 0x00, 0x3B,
    0x00, 0x39, 0x00, 0xF8, 0x00, 0xF8, 0x03, 0x00, 0x07, 0x00, 0x0F, 0x00,
    0xBF, 0x00, 0xFB, 0x00, 0xF3, 0x00, 0xE3, 0x00, 0x43, 0xE0, 0x00, 0xE0,
    0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xE0, 0x00, 0xE0};

chip8_t initChip8() {
  static struct chip8 consoleBlock = {0};
  chip8_t console = &consoleBlock;
  memset(console, 0, sizeof(struct chip8));
  memcpy(console->memory + 0x50, font, 80);

  /* fake rom loading */
  memcpy(console->memory + 0x200, rom_ibm, 132);

  console->pc = 0x200;
  return console;
}

void cycle(chip8_t c) {
  int idx; /* TODO break out reg<->mems */
  uint16_t opCode = c->memory[c->pc] << 8 | c->memory[c->pc + 1];

  c->pc += 2;
  if (c->sound > 0) {
    c->sound--;
  }
  if (c->delay > 0) {
    c->delay--;
  }

  switch (opCode & 0xF000) {
  case 0x000:
    switch
      K {
      case 0x00E0:
        memset(c->display, 0, 2048);
        c->memory[0] = 1;
        break;
      case 0x00EE:
        c->sp--;
        c->pc = c->stack[c->sp];
        break;
      default:
        os_PutStrFull("machine code jumps not supported ");
        exit(-1);
      }
    break;
  case 0x1000:
    c->pc = N;
    break;
  case 0x2000:
    c->stack[c->sp] = c->pc;
    c->sp++;
    c->pc = N;
    break;
  case 0x3000:
    if (c->regs[X] == K) {
      c->pc += 2;
    }
    break;
  case 0x4000:
    if (c->regs[X] != K) {
      c->pc += 2;
    }
    break;
  case 0x5000:
    if (c->regs[X] == c->regs[Y]) {
      c->pc += 2;
    }
    break;
  case 0x6000:
    c->regs[X] = K;
    break;
  case 0x7000:
    c->regs[X] += K;
    break;
  case 0x8000:
    switch (opCode & 0xF) {
    case 0x0:
      c->regs[X] = c->regs[Y];
      break;
    case 0x1:
      c->regs[X] |= c->regs[Y];
      break;
    case 0x2:
      c->regs[X] &= c->regs[Y];
      break;
    case 0x3:
      c->regs[X] ^= c->regs[Y];
      break;
    case 0x4:
      c->regs[X] += c->regs[Y];
      c->regs[0xF] = c->regs[Y] > (0xFF - c->regs[X]);
      break;
    case 0x5:
      c->regs[0xF] = !(c->regs[Y] > c->regs[X]);
      c->regs[X] -= c->regs[Y];
      break;
    case 0x6:
      c->regs[0xF] = c->regs[X] & 0x01;
      c->regs[X] >>= 1;
      break;
    case 0x7:
      c->regs[0xF] = !(c->regs[X] > c->regs[Y]);
      c->regs[X] = c->regs[Y] - c->regs[X];
      break;
    case 0xE:
      c->regs[0xF] = c->regs[X] >> 7;
      c->regs[X] <<= 1;
      break;
    default:
      os_PutStrFull("bad op");
      exit(-1);
    }
    break;
  case 0x9000:
    if (c->regs[X] != c->regs[Y]) {
      c->pc += 2;
    }
    break;
  case 0xA000:
    c->i = N;
    break;
  case 0xB000:
    c->pc = N + c->regs[0];
    break;
  case 0xC000:
    c->regs[X] = K & (rand() % 256);
    break;
  case 0xD000: {
    dxyn(c, opCode);
    c->memory[0] = 1;
    break;
  case 0xE000:
    switch
      K {
      case 0x9E:
        if (c->keypad[c->regs[X]]) {
          c->pc += 2;
        }
        break;
      case 0xA1:
        if (!c->keypad[c->regs[X]]) {
          c->pc += 2;
        }
        break;
      default:
        os_PutStrFull("bad op");
        exit(-1);
      }
    break;
  case 0xF000:
    switch
      K {
      case 0x07:
        c->regs[X] = c->delay;
        break;
      case 0x0A:
        for (idx = 0; idx < 16; idx++) {
          if (c->keypad[idx]) {
            c->regs[X] = idx;
            return; /* !!! */
          }
        }
        c->sp -= 2;
        break;
      case 0x15:
        c->delay = c->regs[X];
        break;
      case 0x18:
        c->sound = c->regs[X];
        break;
      case 0x1E:
        c->i += c->regs[X];
        break;
      case 0x29:
        c->i = (c->regs[X] * 5) + 0x50;
        break;
      case 0x33:
        c->memory[c->i] = c->regs[X] / 100;
        c->memory[c->i + 1] = (c->regs[X] / 10) % 10;
        c->memory[c->i + 2] = c->regs[X] % 10;
        break;
      case 0x55:
        for (idx = 0; idx <= (X); ++idx) {
          c->memory[c->i + idx] = c->regs[idx];
        }
        c->i += (X) + 1; /* RETRO */
        break;
      case 0x65:
        for (idx = 0; idx <= (X); ++idx) {
          c->regs[idx] = c->memory[c->i + idx];
        }
        c->i += (X) + 1; /* RETRO */
        break;
      default:
        os_PutStrFull("bad op");
        exit(-1);
      }
    break;
  }
  default:
    os_PutStrFull("bad op");
    exit(-1);
  }
}

void dxyn(chip8_t console, uint16_t opCode) {
  uint16_t xPos, yPos, spriteSize;
  uint8_t yIdx, xIdx, curByte;

  xPos = console->regs[X];
  yPos = console->regs[Y];
  spriteSize = opCode & 0x00F;
  console->regs[0xF] = 0;
  for (yIdx = 0; yIdx < spriteSize; yIdx++) {
    curByte = console->memory[console->i + yIdx];
    for (xIdx = 0; xIdx < 8; xIdx++) {
      if (curByte & (0x80 >> xIdx)) {
        console->regs[0xF] |=
            !(console->display[(xPos + xIdx) + (64 * (yPos + yIdx))] ^= 1);
      }
    }
  }
}
