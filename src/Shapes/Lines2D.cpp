#include "Shapes/Lines2D.h"

#include <math.h>

#include "Utils/Utils.h"

Line2D::Line2D() : Line2D(0.f,0.f,0.f,0.f)
{

}

Line2D::Line2D(float x0, float y0, float x1, float y1) : Line2D(Vec2D(x0, y0), Vec2D(x1, y1))
{

}

Line2D::Line2D(const Vec2D & p0, const Vec2D & p1): mPoint0(p0), mPoint1(p1)
{

}

bool Line2D::operator==(const Line2D & lineLHS) const
{
	return ((mPoint0 == lineLHS.mPoint0) && (mPoint1 == lineLHS.mPoint1));
}

float Line2D::MinDistanceFrom(const Vec2D & point, bool limitToSegment) const
{
	return point.Distance(ClosestPoint(point, limitToSegment));
}

Vec2D Line2D::ClosestPoint(const Vec2D & point, bool limitToSegment) const
{
	Vec2D Point0ToPoint = point - mPoint0;
	Vec2D unitVecP0ToP1 = (mPoint0 - mPoint1).GetUnitVector();


	float dot = Point0ToPoint.Dot(unitVecP0ToP1);

	if (limitToSegment)
	{
		dot = fmax(0.f, fmin(dot, Length()));
	}
	

	return mPoint0 + dot * unitVecP0ToP1;
}

Vec2D Line2D::MidPoint() const
{
	return Vec2D(0.5 * (mPoint0 + mPoint1));
}

float Line2D::Slope() const
{
	Vec2D diff = mPoint1 - mPoint0;
	if (IsEqual(diff.GetX(), 0.f))
	{
		return INFINITY;
	}
	return diff.GetY()/diff.GetX();
}

float Line2D::Length() const
{
	return mPoint1.Distance(mPoint0);
}

Line2D Line2D::Rotate(Vec2D aroundPoint, float byAngle)
{
	mPoint0.Rotate(aroundPoint, byAngle);
	mPoint1.Rotate(aroundPoint, byAngle);

	return *this;
}

Line2D Line2D::RotationResult(Vec2D aroundPoint, float byAngle)
{
	return Line2D(mPoint0.RotationResult(aroundPoint, byAngle), mPoint1.RotationResult(aroundPoint, byAngle));
}