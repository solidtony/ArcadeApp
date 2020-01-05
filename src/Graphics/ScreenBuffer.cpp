#include "Graphics/ScreenBuffer.h"

#include "SDL.h"
#include <cassert>

#include "Graphics/Color.h"

ScreenBuffer::ScreenBuffer(): mSurface(nullptr) {}
ScreenBuffer::ScreenBuffer(const ScreenBuffer& screenBuffer)
{
	mSurface = SDL_CreateRGBSurfaceWithFormat(0, screenBuffer.mSurface->w,
		screenBuffer.mSurface->h, 0, screenBuffer.mSurface->format->format);

	SDL_BlitSurface(screenBuffer.mSurface, nullptr, mSurface, nullptr);
}

ScreenBuffer::~ScreenBuffer()
{
	if(mSurface != nullptr)
	{
		SDL_FreeSurface(mSurface);
	}
}

ScreenBuffer& ScreenBuffer::operator=(const ScreenBuffer& screenBuffer)
{
	if (this == &screenBuffer)
	{
		return *this;
	}

	if (mSurface != nullptr)
	{
		SDL_FreeSurface(mSurface);
		mSurface = nullptr;
	}

	if (screenBuffer.mSurface != nullptr)
	{
		mSurface = SDL_CreateRGBSurfaceWithFormat(0, screenBuffer.mSurface->w,
			screenBuffer.mSurface->h, 0, screenBuffer.mSurface->format->format);

		SDL_BlitSurface(screenBuffer.mSurface, nullptr, mSurface, nullptr);
	}

	return *this;
}

void ScreenBuffer::SetPixel(const Color& color, int x, int y)
{
	assert((mSurface != nullptr));
	bool bAreInputsValid = (mSurface && (y < mSurface->h) && (y >= 0) && (x >= 0) && (x < mSurface->w));
	if (!bAreInputsValid) { return; }
	// Exlusive access to surface
	SDL_LockSurface(mSurface);

	uint32_t* pixels = (uint32_t*)mSurface->pixels;
	uint32_t index = GetIndex(y, x);
	pixels[index] = color.GetPixelColor();

	SDL_UnlockSurface(mSurface);
}

uint32_t ScreenBuffer::GetIndex(int row, int column)
{
	assert((mSurface != nullptr));
	if (mSurface == nullptr) { return 0; }
	return row * mSurface->w + column;
}

void ScreenBuffer::Init(uint32_t format, uint32_t width, uint32_t height)
{
	mSurface = SDL_CreateRGBSurfaceWithFormat(0, width,
		height, 0, format);
	Clear(); // Sets the screen to black
}

void ScreenBuffer::Clear(const Color& color)
{
	assert((mSurface != nullptr));

	if (mSurface != nullptr)
	{
		SDL_FillRect(mSurface, nullptr, color.GetPixelColor());
	}
}