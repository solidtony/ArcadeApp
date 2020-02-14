#include "Shapes/Shape.h"

void Shape::MoveBy(const Vec2D& deltaOffset)
{
	for (Vec2D& point : mPoints)
	{
		point = point + deltaOffset;
	}
}

void Shape::MoveTo(Vec2D position)
{
	Vec2D center = GetCentroid();

	MoveBy(position - center);
}

Vec2D Shape::GetCentroid() const
{
	if (mPoints.size() == 0) { return Vec2D::Zero(); }

	Vec2D sum = Vec2D::Zero();
	for (const Vec2D& point : mPoints)
	{
		sum += point;
	}

	return sum / (float)mPoints.size();
}

void Shape::Rotate(Vec2D aroundPoint, float byAngle)
{
	for (auto &point : mPoints)
	{
		point.Rotate(aroundPoint, byAngle);
	}
}