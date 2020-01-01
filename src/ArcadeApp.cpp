#include <iostream>
#include "SDL.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;

void SetPixel(SDL_Surface* noptrWindowSurface, uint32_t color, int x, int y);
size_t GetIndex(SDL_Surface* noptrSurface, int row, int column);


int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "SDL_Init Failed with SDL Error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Window* optrWindow = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	if (optrWindow == nullptr)
	{
		std::cout << "Could not create window, get error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Surface* noptrWindowSurface = SDL_GetWindowSurface(optrWindow); // 2D array of pixles

	// Pixel format
	SDL_PixelFormat* pixelFormat = noptrWindowSurface->format;
	std::cout << "The window pixel format is: " << SDL_GetPixelFormatName(pixelFormat->format);

	uint32_t color = 0xFF00FF;
	SetPixel(noptrWindowSurface, color, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	SDL_UpdateWindowSurface(optrWindow);


	SDL_Event sdlEvent;
	bool running = true;

	while (running)
	{
		while (SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}
	}

	SDL_DestroyWindow(optrWindow);
	SDL_Quit();

	return EXIT_SUCCESS;
}

void SetPixel(SDL_Surface* noptrWindowSurface, uint32_t color, int row, int column)
{
	// Exlusive access to surface
	SDL_LockSurface(noptrWindowSurface);

	uint32_t* pixels = (uint32_t*)noptrWindowSurface->pixels;
	size_t index = GetIndex(noptrWindowSurface, column, row);
	pixels[index] = color;

	SDL_UnlockSurface(noptrWindowSurface);
}
size_t GetIndex(SDL_Surface* noptrSurface, int row, int column)
{
	return row * noptrSurface->w + column;
}