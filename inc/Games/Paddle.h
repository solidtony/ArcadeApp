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

static Color PADDLE_COLOR = Color::Blue();

class Paddle : public Excluder
{
public:
	void Init(const AARectangle& rect);
	void Update(uint32_t dt);
	void Draw(Screen& screen);
	inline bool IsMovingLeft() const { return mDirection == PaddleDirection::LEFT; }
	inline bool IsMovingRight() const { return mDirection == PaddleDirection::RIGHT; }

	inline void SetMovementDirection(PaddleDirection dir) { mDirection = dir; }
	inline void StopMovement() { mDirection = PaddleDirection::NONE; }

private:
	static constexpr float VELOCITY = 50;

	PaddleDirection mDirection; // direction we're moving
};