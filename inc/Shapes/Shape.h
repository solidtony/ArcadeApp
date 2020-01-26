#pragma once

#include <vector>
#include "Utils/Vec2D.h"

class Shape
{
public:
	virtual ~Shape() {}
	virtual Vec2D GetCenterPoint() const = 0;
	inline virtual std::vector<Vec2D> GetPoint() const { return mPoints; }
	void MoveBy(const Vec2D& deltaOffset);

protected:
	std::vector<Vec2D> mPoints;
	Vec2D virtual GetCentroid() const;
};