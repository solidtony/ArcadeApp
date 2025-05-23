#include "Graphics/Screen.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstring>
#include <SDL2/SDL.h>

#include "Graphics/BitmapFont.h"
#include "Graphics/BMPImage.h"
#include "Graphics/SpriteSheet.h"
#include "Shapes/Lines2D.h"
#include "Shapes/Triangle.h"
#include "Shapes/AARectangle.h"
#include "Shapes/Circle.h"
#include "Shapes/Star.h"
#include "Utils/Vec2D.h"
#include "Utils/Utils.h"

Screen::Screen() : mWidth(0), mHeight(0), moptrWindow(nullptr), mnoptrWindowSurface(nullptr), mRenderer(nullptr), mPixelFormat(nullptr), mTexture(nullptr)
{

}

Screen::~Screen()
{
	if (mPixelFormat)
	{
		SDL_FreeFormat(mPixelFormat);
		mPixelFormat = nullptr;
	}

	if (mTexture)
	{
		SDL_DestroyTexture(mTexture);
		mPixelFormat = nullptr;
	}

	if (mRenderer)
	{
		SDL_DestroyRenderer(mRenderer);
		mRenderer = nullptr;
	}

	if (moptrWindow != nullptr)
	{
		SDL_DestroyWindow(moptrWindow);
		moptrWindow = nullptr;
	}
	SDL_Quit();
}

SDL_Window* Screen::Init(uint32_t width, uint32_t height, uint32_t mag, bool fast)
{
	mFast = fast;
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

	uint8_t rClear = 0;
	uint8_t gClear = 0;
	uint8_t bClear = 0;
	uint8_t aClear = 255;

	if (mFast)
	{
		mRenderer = SDL_CreateRenderer(moptrWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (mRenderer == nullptr)
		{
			std::cout << "SDL_CreateRenderer failed" << std::endl;
			return nullptr;
		}

		SDL_SetRenderDrawColor(mRenderer, rClear, gClear, bClear, aClear);
	}
	else
	{
		mnoptrWindowSurface = SDL_GetWindowSurface(moptrWindow); // 2D array of pixles
	}

	mPixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB8888);

	if (mFast)
	{
		mTexture = SDL_CreateTexture(mRenderer, mPixelFormat->format, SDL_TEXTUREACCESS_STREAMING, width, height);
	}

	// Pixel format
	Color::InitColorFormat(mPixelFormat);

	mClearColor = Color(rClear, gClear, bClear, aClear);
	mBackBuffer.Init(mPixelFormat->format, mWidth, mHeight);
	mBackBuffer.Clear(mClearColor);

	return moptrWindow;
}

void Screen::SwapScreens()
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }

	ClearScreen();

	if (mFast)
	{
		uint8_t* textureData = nullptr;
		int texturePitch = 0;

		if (SDL_LockTexture(mTexture, nullptr, (void**)&textureData, &texturePitch) >= 0)
		{
			SDL_Surface* surface = mBackBuffer.GetSurface();
			std::memcpy(textureData, surface->pixels, surface->w * surface->h * mPixelFormat->BytesPerPixel);

			SDL_UnlockTexture(mTexture);
			SDL_RenderCopy(mRenderer, mTexture, nullptr, nullptr);
			SDL_RenderPresent(mRenderer);
		}
	}
	else
	{
		SDL_BlitScaled(mBackBuffer.GetSurface(), nullptr, mnoptrWindowSurface, nullptr);

		SDL_UpdateWindowSurface(moptrWindow);
	}




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
	mBackBuffer.SetPixel(color, static_cast<int>(point.GetX()), static_cast<int>(point.GetY()));
}

void Screen::Draw(const Line2D & line, const Color & color)
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }

	int dx, dy;

	int x0 = static_cast<int>(roundf(line.GetPoint0().GetX()));
	int y0 = static_cast<int>(roundf(line.GetPoint0().GetY()));
	int x1 = static_cast<int>(roundf(line.GetPoint1().GetX()));
	int y1 = static_cast<int>(roundf(line.GetPoint1().GetY()));

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

void Screen::Draw(const Triangle & triangle, const Color & color, bool fill, const Color& fillColor)
{
	if (fill)
	{
		FillPoly(triangle.GetPoints(), [fillColor](uint32_t x, uint32_t y) { return fillColor; });
	}

	Line2D p0p1 = Line2D(triangle.GetP0(), triangle.GetP1());
	Line2D p1p2 = Line2D(triangle.GetP1(), triangle.GetP2());
	Line2D p2p0 = Line2D(triangle.GetP2(), triangle.GetP0());

	Draw(p0p1, color);
	Draw(p1p2, color);
	Draw(p2p0, color);
}

void Screen::Draw(const AARectangle& rect, const Color& color, bool fill, const Color& fillColor)
{
	if (fill)
	{
		FillPoly(rect.GetPoints(), [fillColor](uint32_t x, uint32_t y) { return fillColor; });
	}

	std::vector<Vec2D> points = rect.GetPoints();

	const unsigned int NUMBER_OF_POINTS = 4;
	Line2D rectPoints[NUMBER_OF_POINTS];

	rectPoints[0] = Line2D(points[0], points[1]);
	rectPoints[1] = Line2D(points[1], points[2]);
	rectPoints[2] = Line2D(points[2], points[3]);
	rectPoints[3] = Line2D(points[3], points[0]);

	for (int point = 0; point < NUMBER_OF_POINTS; ++point)
	{
		Draw(rectPoints[point], color);
	}
}

void Screen::Draw(const Circle& circle, const Color& color, bool fill, const Color& fillColor)
{
	static unsigned int NUM_CIRCLE_SEGMENTS = 30;

	std::vector<Vec2D> circlePoints;
	std::vector<Line2D> lines;

	float angle = math::constants::TWO_PI_F / float(NUM_CIRCLE_SEGMENTS);

	Vec2D p0 = Vec2D(circle.GetCenterPoint().GetX() + circle.GetRadius(), circle.GetCenterPoint().GetY());
	Vec2D p1 = p0;
	Line2D nextLineToDraw;

	for (unsigned int i = 0; i < NUM_CIRCLE_SEGMENTS; ++i)
	{
		p1.Rotate(circle.GetCenterPoint(), angle);
		nextLineToDraw.SetPoint1(p1);
		nextLineToDraw.SetPoint0(p0);

		lines.push_back(nextLineToDraw);
		p0 = p1;
		circlePoints.push_back(p0);
	}

	if (fill)
	{
		FillPoly(circlePoints, [fillColor](uint32_t x, uint32_t y) { return fillColor; });
	}

	for (const Line2D& line : lines)
	{
		Draw(line, color);
	}
}


void Screen::Draw(const Star& star, const Color& color, bool fill, const Color& fillColor)
{
	if (fill)
	{
		FillPoly(star.GetPoints(), [fillColor](uint32_t x, uint32_t y) { return fillColor; });
	}

	assert(moptrWindow);

	if (moptrWindow == nullptr) { return; }
	for (size_t i = 0; i < star.GetPoints().size() - 1; i++)
	{
		Draw(Line2D(star.GetPoints()[i], star.GetPoints()[i + 1]), color);
	}
	// Draw the last side
	Draw(Line2D(star.GetPoints().back(), star.GetPoints()[0]), color);

}

void Screen::Draw(const BMPImage& image, const Sprite& sprite, const Vec2D& pos, const Color& overlayColor)
{
	float rVal = static_cast<float>(overlayColor.GetRed()) / 255.f;
	float gVal = static_cast<float>(overlayColor.GetGreen()) / 255.f;
	float bVal = static_cast<float>(overlayColor.GetBlue()) / 255.f;
	float aVal = static_cast<float>(overlayColor.GetAlpha()) / 255.f;


	uint32_t width = sprite.width;
	uint32_t height = sprite.height;

	const std::vector<Color>& pixels = image.GetPixels();

	Vec2D topLeft = pos;
	Vec2D topRight = pos + Vec2D(width, 0);
	Vec2D bottomLeft = pos + Vec2D(0, height);
	Vec2D bottomRight = pos + Vec2D(width, height);

	std::vector<Vec2D> points = { topLeft, bottomLeft, bottomRight, topRight };

	Vec2D xAxis = topRight - topLeft;
	Vec2D yAxis = bottomLeft - topLeft;

	const float invXAxisLengthSq = 1.0f / xAxis.Mag2();
	const float invYAxisLengthSq = 1.0f / yAxis.Mag2();

	FillPoly(points, [&](uint32_t px, uint32_t py)
	{
		Vec2D p = { static_cast<float>(px), static_cast<float>(py) };
		Vec2D d = p - topLeft;

		float u = invXAxisLengthSq * d.Dot(xAxis);
		float v = invYAxisLengthSq * d.Dot(yAxis);

		u = Clamp(u, 0.0f, 1.0f);
		v = Clamp(v, 0.0f, 1.0f);

		float tx = roundf(u * static_cast<float>(sprite.width));
		float ty = roundf(v * static_cast<float>(sprite.height));

		Color imageColor = pixels[GetIndex(image.GetWidth(), ty + sprite.yPos, tx + sprite.xPos)];

		Color newColor = { static_cast<uint8_t>(imageColor.GetRed()*rVal), static_cast<uint8_t>(imageColor.GetGreen()*gVal), static_cast<uint8_t>(imageColor.GetBlue()*bVal), static_cast<uint8_t>(imageColor.GetAlpha()*aVal) };

		return newColor;
	});
}

void Screen::Draw(const SpriteSheet& spriteSheet, const std::string& spriteName, const Vec2D& pos, const Color& overlayColor)
{
	Draw(spriteSheet.GetBMPImage(), spriteSheet.GetSprite(spriteName), pos, overlayColor);
}

void Screen::Draw(const BitmapFont& font, const std::string& textLine, const Vec2D& atPosition, const Color& overlayColor)
{
	uint32_t xPos = atPosition.GetX();
	const SpriteSheet& ss = font.GetSpriteSheet();

	for (char c : textLine)
	{
		if (c == ' ')
		{
			xPos += font.GetFontSpacingBetweenWords();
			continue;
		}

		Sprite sprite = ss.GetSprite(std::string("") + c);

		Draw(ss.GetBMPImage(), sprite, Vec2D(xPos, atPosition.GetY()), overlayColor);

		xPos += sprite.width;

		xPos += font.GetFontSpacingBetweenLetters();
	}
}

void Screen::ClearScreen()
{
	assert(moptrWindow);
	if (moptrWindow == nullptr) { return; }

	if (mFast)
	{
		SDL_RenderClear(mRenderer);
	}
	else
	{
		SDL_FillRect(mnoptrWindowSurface, nullptr, mClearColor.GetPixelColor());
	}
}

void Screen::FillPoly(const std::vector<Vec2D>& points, FillPolyFunc func)
{
	if (points.size() > 0)
	{
		float top = points[0].GetY();
		float bottom = points[0].GetY();
		float right = points[0].GetX();
		float left = points[0].GetX();

		for (size_t i = 1; i < points.size(); ++i)
		{
			if (points[i].GetY() < top)
			{
				top = points[i].GetY();
			}

			if (points[i].GetY() > bottom)
			{
				bottom = points[i].GetY();
			}

			if (points[i].GetX() < left)
			{
				left = points[i].GetX();
			}

			if (points[i].GetX() > right)
			{
				right = points[i].GetX();
			}
		}


		for (int pixelY = top; pixelY < bottom; ++pixelY)
		{
			std::vector<float> nodeXVec;

			size_t j = points.size() - 1;

			for (size_t i = 0; i < points.size(); ++i)
			{
				float pointiY = points[i].GetY();
				float pointjY = points[j].GetY();


				if ((pointiY <= (float)pixelY && pointjY > (float)pixelY) || (pointjY <= (float)pixelY && pointiY > (float)pixelY))
				{
					float denom = pointjY - pointiY;
					if (IsEqual(denom, 0))
					{
						continue;
					}

					float x = points[i].GetX() + (pixelY - pointiY) / (denom) * (points[j].GetX() - points[i].GetX());
					nodeXVec.push_back(x);
				}

				j = i;
			}

			std::sort(nodeXVec.begin(), nodeXVec.end(), std::less<>());

			for (size_t k = 0; k < nodeXVec.size(); k += 2)
			{
				if (nodeXVec[k] > right)
				{
					break;
				}

				if (nodeXVec[k + 1] > left)
				{
					if (nodeXVec[k] < left)
					{
						nodeXVec[k] = left;
					}
					if (nodeXVec[k + 1] > right)
					{
						nodeXVec[k + 1] = right;
					}

					//Line2D line = {Vec2D(nodeXVec[k], pixelY), Vec2D(nodeXVec[k+1], pixelY)};
					//Draw(line, color);
					for (int pixelX = nodeXVec[k]; pixelX < nodeXVec[k + 1]; ++pixelX)
					{
						Draw(pixelX, pixelY, func(pixelX, pixelY));
					}
				}
			}
		}
	}
}
