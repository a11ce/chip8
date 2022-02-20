#include "graphics-ce.h"
#include <graphx.h>

gfx_ctx_t initGFX() {
  gfx_Begin();
  gfx_SetDrawBuffer();
  gfx_palette[1] = gfx_RGBTo1555(161, 28, 224);
  gfx_SetColor(0);
  gfx_FillRectangle(0, 0, 320, 240);
}

void render(chip8_t console, gfx_ctx_t gfxCtx) {
  int idx;
  int idy;

  if (console->memory[0]) {
    console->memory[0] = 0;
    gfx_BlitScreen();
    for (idx = 0; idx < 64; idx++) {
      for (idy = 0; idy < 32; idy++) {
        gfx_SetColor(console->display[idx + (64 * idy)]);
        gfx_FillRectangle_NoClip(idx * 5, 40 + (idy * 5), 5, 5);
      }
    }
    gfx_SwapDraw();
  }
}

int procInput(chip8_t console, gfx_ctx_t gfxCtx) { return 1; }

void frameDelay(gfx_ctx_t ctx) { return; }

void endGFX(gfx_ctx_t ctx) { gfx_End(); }
