all: chip8-sdl chip8-cli
.PHONY: all

chip8-sdl: main.c chip8.c graphics-sdl.c
	cc -ansi -pedantic -Wall -Wextra -isystem /usr/local/include -L/usr/local/lib -lSDL2 main.c chip8.c graphics-sdl.c -o bin/chip8-sdl
	
chip8-cli: main.c chip8.c graphics-cli.c
	cc -ansi -pedantic -Wall -Wextra -isystem /usr/local/include -L/usr/local/lib -lSDL2 main.c chip8.c graphics-cli.c -o bin/chip8-cli

