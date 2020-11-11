#include "Games/Paddle.h"

#include "Games/BoundaryEdge.h"
#include "Games/Ball.h"

#include "Graphics/Screen.h"

#include "Utils/Utils.h"

#include <cassert>

void Paddle::Init(const AARectangle& rect, const AARectangle& boundary)
{
	Excluder::Init(rect);
	mBoundary = boundary;
	mDirection = 0;
}

void Paddle::Update(uint32_t dt, Ball& ball)
{
	if (GetAARectangle().ContainsPoint(ball.GetPosition()))
	{
		Vec2D pointOnEdge;
		ball.MakeFlushWithEdge(GetEdge(BOTTOM_EDGE), pointOnEdge, true);
	}

	if (mDirection != 0)
	{
		Vec2D dir;
		if (((mDirection & PaddleDirection::LEFT) == PaddleDirection::LEFT) && ((mDirection & PaddleDirection::RIGHT) == PaddleDirection::RIGHT))
		{
			dir = Vec2D::Zero();
		}
		else if (mDirection == PaddleDirection::LEFT)
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
			MoveTo(Vec2D(mBoundary.GetBottomRightPoint().GetX() - aaRect.GetWidth(), aaRect.GetTopLeftPoint().GetY()));
		}
	}
}

void Paddle::Draw(Screen& screen)
{
	screen.Draw(GetAARectangle(), Color::Blue(), true, Color::Blue());
}

bool Paddle::Bounce(Ball& ball)
{
	BoundaryEdge edge; // Edge that was the ball collided with
	if (HasCollided(ball.GetBoundingRect(), edge))
	{
		Vec2D pointOnEdge;
		ball.MakeFlushWithEdge(edge, pointOnEdge, true);

		if (edge.edge == GetEdge(TOP_EDGE).edge)
		{
			float edgeLength = edge.edge.Length();

			assert(!IsEqual(edgeLength, 0.f));
			float topX = (pointOnEdge.GetX() - edge.edge.GetPoint0().GetX()) / edgeLength;

			if (((topX <= CORNER_BOUNCE_AMT) && (ball.GetVelocity().GetX() > 0.f))
				|| ((topX >= (1.f - CORNER_BOUNCE_AMT)) && (ball.GetVelocity().GetX() < 0.f)))
			{
				ball.SetVelocity(-ball.GetVelocity());
				return true;
			}

			ball.SetVelocity(ball.GetVelocity().Reflect(edge.normal));
			return true;
		}
	}

	return false;
}