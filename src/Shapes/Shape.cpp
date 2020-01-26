#include "Shapes/Shape.h"

void Shape::MoveBy(const Vec2D& deltaOffset)
{
	for (Vec2D& point : mPoints)
	{
		point = point + deltaOffset;
	}
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
