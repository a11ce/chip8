#include <stdio.h>
#include <unistd.h>

#include "graphics-cli.h"

gfx_ctx_t initGFX() { return 0; }

void render(chip8_t console, gfx_ctx_t gfxCtx) {
  int idx;
  (void)gfxCtx;
  if (console->memory[0]) {
    console->memory[0] = 0;
    printf("\033[0;0H\033[2J");
    for (idx = 0; idx < 2048; idx++) {
      if (idx % 64 == 0) {
        putchar('\n');
      }
      printf(console->display[idx] ? "█" : " ");
    }
  }
}

void frameDelay(gfx_ctx_t ctx) {
  (void)ctx;
  usleep(1670);
}

void endGFX(gfx_ctx_t ctx) {
  (void)ctx;
  return;
}
