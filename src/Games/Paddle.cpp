#include "Games/Paddle.h"

#include "Games/BoundaryEdge.h"

#include "Graphics/Screen.h"

#include "Utils/Utils.h"

void Paddle::Init(const AARectangle& rect)
{
	Excluder::Init(rect);
	mDirection = PaddleDirection::NONE;
}

void Paddle::Update(uint32_t dt)
{
	if (mDirection != PaddleDirection::NONE)
	{
		Vec2D dir;
		if (mDirection == PaddleDirection::LEFT)
		{
			dir = LEFT_DIR;
		}
		else
		{
			dir = RIGHT_DIR;
		}

		Vec2D dx = dir * VELOCITY * MillisecondsToSeconds(dt);

		MoveBy(dx);
	}
}

void Paddle::Draw(Screen& screen)
{
	screen.Draw(GetAARectangle(), PADDLE_COLOR, true, PADDLE_COLOR);
}

void Paddle::SetMovementDirection(PaddleDirection dir)
{

}