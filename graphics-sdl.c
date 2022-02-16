#include "graphics-sdl.h"

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

void frameDelay(gfx_ctx_t ctx) {
  (void)ctx;
  SDL_Delay(1);
}

void endGFX(gfx_ctx_t ctx) {
  free(ctx);
  SDL_Quit();
}
