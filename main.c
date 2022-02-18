#include "SDL2/SDL.h"
#include "chip8.h"
#include "graphics-cli.h"

int main(int argc, char *argv[]) {

  chip8_t console;
  gfx_ctx_t gfxCtx;

  if (argc < 2) {
    printf("usage:\n\t%s [rom-file]\n", argv[0]);
    exit(1);
  }

  console = initChip8();
  gfxCtx = initGFX();
  srand(time(NULL));

  loadRom(console, argv[1]);

  while (procInput(console, gfxCtx)) {
    frameDelay(gfxCtx);
    cycle(console);
    render(console, gfxCtx);
  }
  endGFX(gfxCtx);

  return 0;
}
