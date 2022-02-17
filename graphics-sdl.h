#pragma once
#include "SDL2/SDL.h"
#include "chip8.h"

typedef struct graphicsCTX *gfx_ctx_t;

struct graphicsCTX {
  SDL_Window *window;
  SDL_Texture *texture;
  SDL_Renderer *renderer;
};

gfx_ctx_t initGFX();
void render(chip8_t console, gfx_ctx_t gfxCtx);
int procInput(chip8_t console, gfx_ctx_t ctx);
void frameDelay(gfx_ctx_t ctx);
void endGFX(gfx_ctx_t ctx);
