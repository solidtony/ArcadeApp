#include "Games/Paddle.h"

#include "Games/BoundaryEdge.h"

#include "Graphics/Screen.h"

#include "Utils/Utils.h"

void Paddle::Init(const AARectangle& rect, const AARectangle& boundary)
{
	Excluder::Init(rect);
	mBoundary = boundary;
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

		const AARectangle& aaRect = GetAARectangle();

		if (IsGreaterThanOrEqual(mBoundary.GetTopLeftPoint().GetX(), aaRect.GetTopLeftPoint().GetX()))
		{
			MoveTo(Vec2D(mBoundary.GetTopLeftPoint().GetX(), aaRect.GetTopLeftPoint().GetY()));
		}
		else if (IsGreaterThanOrEqual(aaRect.GetBottomRightPoint().GetX(), mBoundary.GetBottomRightPoint().GetX()))
		{
			MoveTo(Vec2D(mBoundary.GetBottomRightPoint().GetX() - aaRect.GetWidth()/2.f, aaRect.GetTopLeftPoint().GetY() + aaRect.GetHeight()/2));
		}
	}
}

void Paddle::Draw(Screen& screen)
{
	screen.Draw(GetAARectangle(), Color::Blue(), true, Color::Blue());
}
