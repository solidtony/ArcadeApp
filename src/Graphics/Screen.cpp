#include "Graphics/Screen.h"

#include <cassert>
#include <cmath>
#include "SDL.h"

#include "Shapes/Lines2D.h"
#include "Shapes/Triangle.h"
#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"
#include "Shapes/Star.h"
#include "Utils/Vec2D.h"
#include "Utils/Utils.h"

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
	mBackBuffer.SetPixel(color, (int)point.GetX(), (int)point.GetY());
}

void Screen::Draw(const Line2D & line, const Color & color)
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }
	
	int dx, dy;

	int x0 = (int)roundf(line.GetPoint0().GetX());
	int y0 = (int)roundf(line.GetPoint0().GetY());
	int x1 = (int)roundf(line.GetPoint1().GetX());
	int y1 = (int)roundf(line.GetPoint1().GetY());

	dx = x1 - x0;
	dy = y1 - y0;

	signed const char ix((dx > 0) - (dx < 0)); // evaluate to 1 or -1
	signed const char iy((dy > 0) - (dy < 0));

	dx = abs(dx) * 2;
	dy = abs(dy) * 2;

	Draw(x0, y0, color);

	if (dx >= dy)
	{
		// Run Bresenham's line alg along x
		int d = dy - dx / 2;
		while (x0 != x1)
		{
			if (d >= 0)
			{
				d -= dx;
				y0 += iy;
			}

			d += dy;
			x0 += ix;

			Draw(x0, y0, color);
		}
	}
	else
	{
		// Run Bresenham's line alg along y
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
}

void Screen::Draw(const Triangle & triangle, const Color & color)
{
	Line2D p0p1 = Line2D(triangle.GetP0(), triangle.GetP1());
	Line2D p1p2 = Line2D(triangle.GetP1(), triangle.GetP2());
	Line2D p2p0 = Line2D(triangle.GetP2(), triangle.GetP0());

	Draw(p0p1, color);
	Draw(p1p2, color);
	Draw(p2p0, color);
}

void Screen::Draw(const AARectangle& rect, const Color& color)
{
	std::vector<Vec2D> points = rect.GetPoints();

	const unsigned int NUMBER_OF_POINTS = 4;
	Line2D rectPoints [NUMBER_OF_POINTS];

	rectPoints[0] = Line2D(points[0], points[1]);
	rectPoints[1] = Line2D(points[1], points[2]);
	rectPoints[2] = Line2D(points[2], points[3]);
	rectPoints[3] = Line2D(points[3], points[0]);

	for (int point = 0; point < NUMBER_OF_POINTS; ++point)
	{
		Draw(rectPoints[point], color);
	}
}

void Screen::Draw(const Circle& circle, const Color& color)
{
	static unsigned int NUM_CIRCLE_SECMENTS = 30;

	float angle = math::constants::TWO_PI_F / float(NUM_CIRCLE_SECMENTS);

	Vec2D p0 = Vec2D(circle.GetCenterPoint().GetX() + circle.GetRadius(), circle.GetCenterPoint().GetY());
	Vec2D p1 = p0;
	Line2D nextLineToDraw;

	for (unsigned int i = 0; i < NUM_CIRCLE_SECMENTS; ++i)
	{
		p1.Rotate(circle.GetCenterPoint(), angle);
		nextLineToDraw.SetPoint1(p1);
		nextLineToDraw.SetPoint0(p0);

		Draw(nextLineToDraw, color);

		p0 = p1;
	}
}

void Screen::Draw(const Star& star, const Color& color)
{
	assert(moptrWindow);

	if (moptrWindow == nullptr) { return; }
	for (size_t i = 0; i < star.GetPoints().size() - 1; i++)
	{
		Draw(Line2D(star.GetPoints()[i], star.GetPoints()[i + 1]), color);
	}
	// Draw the last side
	Draw(Line2D(star.GetPoints().back(), star.GetPoints()[0]), color);

}

void Screen::ClearScreen()
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }

	SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());
}