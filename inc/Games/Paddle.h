#pragma once

#include "Excluder.h"

#include <stdint.h>

#include "Graphics/Color.h"

class Screen;
class Color;

enum PaddleDirection
{
	NONE = 0,
	LEFT,
	RIGHT
};

class Paddle : public Excluder
{
public:
	static const uint32_t PADDLE_WIDTH = 50;
	static const uint32_t PADDLE_HEIGHT = 10;

	void Init(const AARectangle& rect, const AARectangle& boundary);
	void Update(uint32_t dt);
	void Draw(Screen& screen);

	inline bool IsMovingLeft() const { return mDirection == PaddleDirection::LEFT; }
	inline bool IsMovingRight() const { return mDirection == PaddleDirection::RIGHT; }

	inline void SetMovementDirection(PaddleDirection dir) { mDirection = dir; }
	inline void StopMovement() { mDirection = PaddleDirection::NONE; }

private:
	static constexpr float VELOCITY = 100.f; // pixels/second
	AARectangle mBoundary; // Boundary that the paddle is confined to
	PaddleDirection mDirection; // direction we're moving
};