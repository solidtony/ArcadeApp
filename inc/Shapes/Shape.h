#pragma once

#include <vector>
#include "Utils/Vec2D.h"

class Shape
{
public:
	virtual ~Shape() {}
	virtual Vec2D GetCenterPoint() const = 0;
	inline virtual const std::vector<Vec2D>& GetPoints() const { return mPoints; }
	void MoveBy(const Vec2D& deltaOffset);
	void Rotate(Vec2D aroundPoint, float byAngle);

protected:
	std::vector<Vec2D> mPoints;
	Vec2D virtual GetCentroid() const;
};