#pragma once

#include "Utils/Vec2D.h"

class Line2D
{
public:
	Line2D();
	Line2D(float x0, float y0, float x1, float y1);
	Line2D(const Vec2D& p0, const Vec2D& p1);

	inline const Vec2D& GetPoint0() const { return mPoint0; }
	inline void SetPoint0(const Vec2D& point) { mPoint0 = point; }
	inline const Vec2D& GetPoint1() const { return mPoint1; }
	inline void SetPoint1(const Vec2D& point) { mPoint1 = point; }

	bool operator==(const Line2D& line) const;
	float MinDistanceFrom(const Vec2D& point, bool limitToSegment = false) const;
	Vec2D ClosestPoint(const Vec2D& point, bool limitToSegment = false) const;
	Vec2D MidPoint() const;
	float Slope() const;
	float Length() const;

	Line2D Rotate(Vec2D aroundPoint, float byAngle);
	Line2D RotationResult(Vec2D aroundPoint, float byAngle);

private:
	Vec2D mPoint0;
	Vec2D mPoint1;
};