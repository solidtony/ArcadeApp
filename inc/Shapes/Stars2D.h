#pragma once

#include <vector>

#include "Shapes/Lines2D.h"
#include "Utils/Vec2D.h"

class Star2D
{
public:
	Star2D();
	Star2D(float centerX, float centerY, float radius, float thickness, unsigned int numberOfPoints);
	Star2D(Vec2D center, float radius, float thickness, unsigned int numberOfPoints);

	~Star2D();

	// getters
	float getRadius() { return mRadius; }
	float getThickness() { return mThickness; }
	unsigned int getNumberOfPoints() { return mNumberOfPoints; }
	Vec2D getCenter() { return mCenter; }
	inline const std::vector<Line2D>& getSides() const { return mSides; }

	Star2D Rotate(Vec2D aroundPoint, float byAngle);

private:
	float mRadius;
	float mThickness;
	unsigned int mNumberOfPoints;
	Vec2D mCenter;
	std::vector<Line2D> mSides = {};
};