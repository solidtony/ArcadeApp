#pragma once

#include <stdint.h>

#include "Graphics/Color.h"
#include "Graphics/ScreenBuffer.h"

struct SDL_Window;
struct SDL_Surface;
class Vec2D;
class Line2D;
class Star2D;

class Screen
{
public:
	Screen();
	~Screen();

	SDL_Window* Init(uint32_t width, uint32_t height, uint32_t mag);
	void SwapScreens();

	inline void SetClearColor(const Color& clearColor) { mClearColor = clearColor; }
	inline uint32_t Width() const { return mWidth; }
	inline uint32_t Height() const { return mHeight; }

	// Draw methods
	void Draw(int x, int y, const Color& color);
	void Draw(const Vec2D& point, const Color& color);
	void Draw(const Line2D& line, const Color& color);
	void Draw(const Star2D& star, const Color& color);

private:
	// Declared in private to prevent copying
	Screen(const Screen& screen);
	Screen& operator=(const Screen& screen);

	void ClearScreen();

	uint32_t mWidth;
	uint32_t mHeight;

	Color mClearColor;
	ScreenBuffer mBackBuffer;

	SDL_Window* moptrWindow;
	SDL_Surface* mnoptrWindowSurface;
};