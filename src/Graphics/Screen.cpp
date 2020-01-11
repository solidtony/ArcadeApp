#include "Graphics/Screen.h"

#include <cassert>
#include <cmath>
#include "SDL.h"

#include "Shapes/Lines2D.h"
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

void Screen::Draw(const Line2D & line, const Color & color)
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }
	
	int dx, dy;

	int x0 = roundf(line.GetPoint0().GetX());
	int y0 = roundf(line.GetPoint0().GetY());
	int x1 = roundf(line.GetPoint1().GetX());
	int y1 = roundf(line.GetPoint1().GetY());

	dx = x1 - x0;
	dy = y1 - y0;

	if (dx*dx >= dy*dy)
	{
		// Run Bresenham's line alg along x
		DrawBLAalongYAxis(y0, y1, x0, x1, color);
	}
	else
	{
		// Run Bresenham's line alg along y
		DrawBLAalongYAxis(x0, x1, y0, y1, color);
	}
}

void Screen::ClearScreen()
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }

	SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());
}

/*
Runs Bresenham's line alg along y
To run alg along x reverse x and y in function call
*/
void Screen::DrawBLAalongYAxis(int x0, int x1, int y0, int y1, Color color)
{
	// Run Bresenham's line alg along y

	int dx = x1 - x0;
	int dy = y1 - y0;

	signed const char ix((dx > 0) - (dx < 0)); // evaluate to 1 or -1
	signed const char iy((dy > 0) - (dy < 0));

	dx = abs(dx) * 2;
	dy = abs(dy) * 2;

	Draw(x0, y0, color);

	int d = dx - dy / 2;
	while (y0 != y1)
	{
		if (d >= 0)
		{
			d -= dy;
			x0 += ix;
		}

		d += dx;
		y0 += iy;

		Draw(x0, y0, color);
	}
}