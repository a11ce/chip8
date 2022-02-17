#pragma once
#include "chip8.h"

typedef struct graphicsCTX *gfx_ctx_t;

struct graphicsCTX {
  int no;
};

gfx_ctx_t initGFX();
void render(chip8_t console, gfx_ctx_t gfxCtx);
int procInput(chip8_t console, gfx_ctx_t ctx);
void frameDelay(gfx_ctx_t ctx);
void endGFX(gfx_ctx_t ctx);
