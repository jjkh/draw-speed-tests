#include <SDL.h>

#pragma comment(lib, "SDL2.lib")

int main(int argc, char** argv)
{
	int exitCode = 1;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        return exitCode;

    SDL_Window* win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (win == NULL)
		return exitCode;
    
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	if (ren == NULL) {
		SDL_DestroyWindow(win);
		SDL_Quit();
		return exitCode;
	}

	bool running = true;
	while (running) {
		if (SDL_SetRenderDrawColor(ren, 0, 83, 112, SDL_ALPHA_OPAQUE) != 0)
			goto cleanup_all;

		if (SDL_RenderClear(ren) != 0)
			goto cleanup_all;

		SDL_RenderPresent(ren);

		SDL_Event event;
		if (SDL_WaitEvent(&event) != 1)
			goto cleanup_all;

		switch (event.type) {
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_ESCAPE) running = false;
		}
	}
	exitCode = 0;

cleanup_all:
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return exitCode;
}
