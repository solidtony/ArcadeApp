#include "Shapes/Triangle.h"

#include <cmath>

#include "Utils/Utils.h"

Triangle::Triangle() : Triangle(Vec2D::Zero(), Vec2D::Zero(), Vec2D::Zero())
{

}

Triangle::Triangle(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2)
{
	mPoints.push_back(p0);
	mPoints.push_back(p1);
	mPoints.push_back(p2);
}

Vec2D Triangle::GetCenterPoint() const
{
	return GetCentroid();
}

float Triangle::Area() const
{
	return Area(GetP0(), GetP1(), GetP2());
}

bool Triangle::ContainsPoint(const Vec2D& p) const
{
	float thisArea = Area();

	float area = 0.f;
	area += Area(p, GetP1(), GetP2());
	area += Area(GetP0(), p, GetP2());
	area += Area(GetP0(), GetP1(), p);

	return IsEqual(thisArea, area);
}


float Triangle::Area(const Vec2D& p0, const Vec2D& p1, const Vec2D& p2) const
{
	// Uses Heron's formula to calculate the area of a triangle
	float area = 0.0f;

	area += p0.GetX()*(p2.GetY() - p1.GetY());
	area += p1.GetX()*(p0.GetY() - p2.GetY());
	area += p2.GetX()*(p1.GetY() - p0.GetY());
	area = fabsf(area);
	area *= 0.5;

	return area;
}
