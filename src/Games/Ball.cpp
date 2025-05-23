#include "Games/Ball.h"

#include "Games/BoundaryEdge.h"
#include "Graphics/Screen.h"
#include "Shapes/Circle.h"
#include "Utils/Utils.h"

const float Ball::RADIUS = 5.f;

Ball::Ball() : Ball(Vec2D::Zero(), RADIUS)
{

}

Ball::Ball(const Vec2D& pos, float radius)
{
	mBBox = AARectangle(pos - Vec2D(radius, radius), static_cast<unsigned int>(radius * 2.f), static_cast<unsigned int>(radius * 2.f));
	mVelocity = Vec2D::Zero();
}

void Ball::Update(uint32_t dt)
{
	mBBox.MoveBy(mVelocity * MillisecondsToSeconds(dt));
}

void Ball::Draw(Screen& screen)
{
	Circle circ = { mBBox.GetCenterPoint(), GetRadius() };
	screen.Draw(circ, Color::Red(), true, Color::Red());
}

void Ball::MakeFlushWithEdge(const BoundaryEdge& edge, Vec2D& pointOnEdgeOUT, bool limitToEdge)
{
	if (edge.normal == DOWN_DIR)
	{
		mBBox.MoveTo(Vec2D(mBBox.GetTopLeftPoint().GetX(), edge.edge.GetPoint0().GetY() + edge.normal.GetY()));
	}
	else if (edge.normal == UP_DIR)
	{
		mBBox.MoveTo(Vec2D(mBBox.GetTopLeftPoint().GetX(), edge.edge.GetPoint0().GetY() - mBBox.GetHeight()));
	}
	else if (edge.normal == RIGHT_DIR)
	{
		mBBox.MoveTo(Vec2D(edge.edge.GetPoint0().GetX() + edge.normal.GetX(), mBBox.GetTopLeftPoint().GetY()));
	}
	else if (edge.normal == LEFT_DIR)
	{
		mBBox.MoveTo(Vec2D(edge.edge.GetPoint0().GetX() - mBBox.GetWidth(), mBBox.GetTopLeftPoint().GetY()));
	}

	pointOnEdgeOUT = edge.edge.ClosestPoint(mBBox.GetCenterPoint(), limitToEdge);
}

void Ball::Bounce(const BoundaryEdge& edge)
{
	Vec2D pointOnEdge;

	MakeFlushWithEdge(edge, pointOnEdge, false);

	mVelocity = mVelocity.Reflect(edge.normal);
}

void Ball::MoveTo(const Vec2D& point)
{
	mBBox.MoveTo(point - Vec2D(GetRadius(), GetRadius()));
}
