#pragma once

#include "Excluder.h"

#include <stdint.h>

#include "Graphics/Color.h"

class Screen;
class Color;
class Ball;

enum PaddleDirection
{
	LEFT = 1 << 0,
	RIGHT = 1 << 1
};

class Paddle : public Excluder
{
public:
	static const uint32_t PADDLE_WIDTH = 50;
	static const uint32_t PADDLE_HEIGHT = 10;

	void Init(const AARectangle& rect, const AARectangle& boundary);
	void Update(uint32_t dt, Ball& ball);
	void Draw(Screen& screen);
	bool Bounce(Ball& ball);

	inline bool IsMovingLeft() const { return mDirection == PaddleDirection::LEFT; }
	inline bool IsMovingRight() const { return mDirection == PaddleDirection::RIGHT; }

	inline void SetMovementDirection(PaddleDirection dir) { mDirection |= dir; }
	inline void UnsetMovementDirection(PaddleDirection dir) { mDirection &= ~dir; }
	inline void StopMovement() { mDirection = 0; }

private:
	static constexpr float VELOCITY = 100.f; // pixels/second
	static constexpr float CORNER_BOUNCE_AMT = 0.2f;
	
	uint32_t mDirection; // direction we're moving
	AARectangle mBoundary; // Boundary that the paddle is confined to
};