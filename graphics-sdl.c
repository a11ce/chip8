#include "graphics-sdl.h"

/* keypad is
/  1 2 3 C
/  4 5 6 D
/  7 8 9 E
/  A 0 B F
/  mapped to
/  1 2 3 4
/  Q W E R
/  A S D F
/  Z X C V  */
char keypad[16] = {'x', '1', '2', '3', 'q', 'w', 'e', 'a',
                   's', 'd', 'z', 'c', '4', 'r', 'f', 'v'};

gfx_ctx_t initGFX() {
  gfx_ctx_t ctx = malloc(sizeof(struct graphicsCTX));
  SDL_Init(SDL_INIT_VIDEO);
  ctx->window =
      SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 1024, 512, SDL_WINDOW_SHOWN);
  ctx->renderer = SDL_CreateRenderer(ctx->window, -1, 0);
  SDL_RenderSetLogicalSize(ctx->renderer, 1024, 512);
  ctx->texture = SDL_CreateTexture(ctx->renderer, SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STREAMING, 64, 32);
  return ctx;
}

void render(chip8_t console, gfx_ctx_t ctx) {
  uint32_t pixels[2048];
  int idx;
  if (console->memory[0]) {
    console->memory[0] = 0;
    for (idx = 0; idx < 2048; ++idx) {
      uint8_t pixel = console->display[idx];
      pixels[idx] = (0x00A11CE0 * pixel) | 0xFF000000;
    }

    SDL_UpdateTexture(ctx->texture, NULL, pixels, 64 * sizeof(uint32_t));
    SDL_RenderClear(ctx->renderer);
    SDL_RenderCopy(ctx->renderer, ctx->texture, NULL, NULL);
    SDL_RenderPresent(ctx->renderer);
  }
}

int procInput(chip8_t console, gfx_ctx_t ctx) {
  SDL_Event cEvent;
  int kIdx;
  (void)console;
  (void)ctx;

  while (SDL_PollEvent(&cEvent)) {
    if (cEvent.type == SDL_QUIT) {
      return 0;
    }
    if (cEvent.type == SDL_KEYDOWN) {
      for (kIdx = 0; kIdx < 16; kIdx++) {
        if (keypad[kIdx] == cEvent.key.keysym.sym) {
          console->keypad[kIdx] = 1;
        }
      }
    } else if (cEvent.type == SDL_KEYUP) {
      for (kIdx = 0; kIdx < 16; kIdx++) {
        if (keypad[kIdx] == cEvent.key.keysym.sym) {
          console->keypad[kIdx] = 0;
        }
      }
    }
  }
  return 1;
}

void frameDelay(gfx_ctx_t ctx) {
  (void)ctx;
  SDL_Delay(2);
}

void endGFX(gfx_ctx_t ctx) {
  free(ctx);
  SDL_Quit();
}

/* UNUSED

void printRegs(chip8_t c) {
  int idx;
  uint16_t opCode = c->memory[c->pc] << 8 | c->memory[c->pc + 1];
  for (idx = 0; idx < 16; idx++) {
    printf("%02x ", c->regs[idx]);
  }
  printf("| %x | %x | ", c->pc, c->i);
  for (idx = 0; idx < 16; idx++) {
    if (c->keypad[idx]) {
      printf("%x", idx);
    } else {
      printf("-");
    }
  }
  printf("\n");
  printf("%04x\n", opCode);
}

void printMem(chip8_t console) {
  int idx;
  for (idx = 0; idx < 4096; idx++) {
    putchar(console->memory[idx]);
  }
}

*/
