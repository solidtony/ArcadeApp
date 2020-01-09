#include "Graphics/Screen.h"

#include <cassert>
#include "SDL.h"
#include "Utils/Vec2D.h"

Screen::Screen() : mWidth(0), mHeight(0), moptrWindow(nullptr), mnoptrWindowSurface(nullptr)
{

}

Screen::~Screen()
{
	if(moptrWindow != nullptr)
	{
		SDL_DestroyWindow(moptrWindow);
		moptrWindow = nullptr;
	}
	SDL_Quit();
}

SDL_Window* Screen::Init(uint32_t width, uint32_t height, uint32_t mag)
{
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "SDL_Init Failed with SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	mWidth = width;
	mHeight = height;

	moptrWindow = SDL_CreateWindow("Arcade", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mag * mWidth, mag * mHeight, 0);

	if (moptrWindow == nullptr)
	{
		std::cout << "Could not create window, get error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	mnoptrWindowSurface = SDL_GetWindowSurface(moptrWindow); // 2D array of pixles

	// Pixel format
	SDL_PixelFormat* pixelFormat = mnoptrWindowSurface->format;

	Color::InitColorFormat(pixelFormat);
	mBackBuffer.Init(pixelFormat->format, mWidth, mHeight);
	mClearColor = Color::Black();
	mBackBuffer.Clear(mClearColor);

	return moptrWindow;
}

void Screen::SwapScreens()
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }
	ClearScreen();

	SDL_BlitScaled(mBackBuffer.GetSurface(), nullptr, mnoptrWindowSurface, nullptr);

	SDL_UpdateWindowSurface(moptrWindow);

	mBackBuffer.Clear(mClearColor);
}

void Screen::Draw(int x, int y, const Color& color)
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }
	mBackBuffer.SetPixel(color, x, y);
}

void Screen::Draw(const Vec2D& point, const Color& color)
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }
	mBackBuffer.SetPixel(color, point.GetX(), point.GetY());
}

void Screen::ClearScreen()
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }

	SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());
}