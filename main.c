#include "SDL2/SDL.h"
#include "chip8.h"
#include "graphics-cli.h"

int main(int argc, char *argv[]) {

  chip8_t console;
  gfx_ctx_t gfxCtx;
  int isQuit = 0;
  SDL_Event cEvent;

  if (argc < 2) {
    printf("usage:\n\t%s [rom-file]\n", argv[0]);
    exit(1);
  }

  console = initChip8();
  gfxCtx = initGFX();
  srand(time(NULL));

  loadRom(console, argv[1]);

  /*
    printMem(console);
   exit(0);
  */
  while (!isQuit) {
    if (SDL_PollEvent(&cEvent)) {
      if (cEvent.type == SDL_QUIT) {
        isQuit = 1;
      }
    }

    cycle(console);
    frameDelay(gfxCtx);
    render(console, gfxCtx);
  }
  endGFX(gfxCtx);

  return 0;
}
