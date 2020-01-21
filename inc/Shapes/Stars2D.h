#pragma once

#include "Shapes/Lines2D.h"
#include "Utils/Vec2D.h"

class Star2D
{
public:
	Star2D();
	Star2D(float centerX, float centerY, float radius, float thickness, unsigned int numberOfArms);
	Star2D(Vec2D center, float radius, float thickness, unsigned int numberOfArms);

	~Star2D();

	// getters
	inline Vec2D getCenter() const { return mCenter; }
	inline float getRadius() const { return mRadius; }
	inline float getThickness() const { return mThickness; }
	inline unsigned int getNumberOfArms() const { return mNumberOfArms; }
	inline unsigned int getNumberOfLines() const { return mNumberOfSides; }
	inline const Line2D* getSides() const { return mSides; }

	void Rotate(Vec2D aroundPoint, float byAngle);

private:
	Vec2D mCenter;
	float mRadius;
	float mThickness;
	unsigned int mNumberOfArms;
	unsigned int mNumberOfSides;
	Line2D* mSides = nullptr;
};