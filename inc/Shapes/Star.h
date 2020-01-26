#pragma once

#include "Shapes/Shape.h"
#include "Shapes/Lines2D.h"
#include "Utils/Vec2D.h"

class Star : public Shape
{
public:
	Star();
	Star(float centerX, float centerY, float radius, float thickness, unsigned int numberOfArms);
	Star(Vec2D center, float radius, float thickness, unsigned int numberOfArms);

	~Star();

	// getters
	inline virtual Vec2D GetCenterPoint() const override { return mCenter; }
	float Radius() const;
	float Thickness() const;
	inline unsigned int NumberOfArms() const { return (unsigned int)mPoints.size()/2; }

private:
	Vec2D mCenter;

	void CalculateStar(Vec2D center, float radius, float thickness, unsigned int numberOfArms);
};