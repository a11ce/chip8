#include "chip8.h"
#include "graphics-ce.h"

int main(void) {
  chip8_t console;
  gfx_ctx_t nullGFX = 0;

  console = initChip8();
  initGFX();

  while (procInput(console, nullGFX)) {
    frameDelay(nullGFX);
    cycle(console);
    render(console, nullGFX);
  }
  endGFX(nullGFX);
  return 0;
}
