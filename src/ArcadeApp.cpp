#include <iostream>
#include "SDL.h"

#include "Graphics/Color.h"
#include "Graphics/ScreenBuffer.h"
#include "Utils/Vec2D.h"

const int SCREEN_WIDTH = 224;
const int SCREEN_HEIGHT = 288;

int main(int argc, char *argv[])
{
	Vec2D vecA(2.0, 4.0);
	Vec2D vecB(1.0, 2.0);
	std::cout << "vecA: " << vecA << "; vecB: " << vecB << std::endl;
	std::cout << "0.0 * vecA: " << 0.0f*vecA << std::endl;
	std::cout << "vecA + vecB: " << vecA + vecB << std::endl;
	std::cout << "vecA - vecB: " << vecA - vecB << std::endl;
	vecB -= vecA;
	std::cout << "vecA: " << vecA << "; vecB: " << vecB << std::endl;
	Vec2D normVec(5.f, 5.f);
	std::cout << "normVec: " << normVec << std::endl;
	std::cout << "Get unit vector: " << normVec.GetUnitVector() << std::endl;
	normVec.Normalize();
	std::cout << "normVec: " << normVec << std::endl;
	Vec2D zeroVec(0.f, 0.f);
	std::cout << "zeroVec: " << zeroVec << std::endl;
	std::cout << "Get unit vector: " << zeroVec.GetUnitVector() << std::endl;
	zeroVec.Normalize();
	std::cout << "zeroVec: " << zeroVec << std::endl;
	std::cout << "Zero magnitude: " << zeroVec.Mag() << std::endl;

	//if (SDL_Init(SDL_INIT_VIDEO))
	//{
	//	std::cout << "SDL_Init Failed with SDL Error: " << SDL_GetError() << std::endl;
	//	return EXIT_FAILURE;
	//}

	//SDL_Window* optrWindow = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	//if (optrWindow == nullptr)
	//{
	//	std::cout << "Could not create window, get error: " << SDL_GetError() << std::endl;
	//	return EXIT_FAILURE;
	//}

	//SDL_Surface* noptrWindowSurface = SDL_GetWindowSurface(optrWindow); // 2D array of pixles

	//// Pixel format
	//SDL_PixelFormat* pixelFormat = noptrWindowSurface->format;
	//std::cout << "The window pixel format is: " << SDL_GetPixelFormatName(pixelFormat->format);

	//Color::InitColorFormat(pixelFormat);

	//ScreenBuffer screenBuffer;

	//screenBuffer.Init(pixelFormat->format, noptrWindowSurface->w, noptrWindowSurface->h);

	//screenBuffer.SetPixel(Color::Red(), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//SDL_BlitSurface(screenBuffer.GetSurfacer(), nullptr, noptrWindowSurface, nullptr);

	//SDL_UpdateWindowSurface(optrWindow);


	//SDL_Event sdlEvent;
	//bool running = true;

	//while (running)
	//{
	//	while (SDL_PollEvent(&sdlEvent))
	//	{
	//		switch (sdlEvent.type)
	//		{
	//		case SDL_QUIT:
	//			running = false;
	//			break;
	//		}
	//	}
	//}

	//SDL_DestroyWindow(optrWindow);
	//SDL_Quit();

	return EXIT_SUCCESS;
}