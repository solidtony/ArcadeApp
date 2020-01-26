#include "Shapes/Star.h"

#include "Utils/Utils.h"

Star::Star() : Star(0.0f, 0.0f, 1.0f, 0.2f, 3)
{

}

Star::Star(float centerX, float centerY, float radius, float thickness, unsigned int numberOfArms) : Star(Vec2D(centerX, centerY), radius, thickness, numberOfArms)
{

}

Star::~Star()
{

}

Star::Star(Vec2D center, float radius, float thickness, unsigned int numberOfArms)
	: mCenter(center)
{
	CalculateStar(center, radius, thickness, numberOfArms);
}

float Star::Radius() const
{
	return (mCenter - mPoints[0]).Mag();
}

float Star::Thickness() const
{
	return (mCenter - mPoints[1]).Mag();
}

void Star::CalculateStar(Vec2D center, float radius, float thickness, unsigned int numberOfArms)
{
	if (numberOfArms < 2)
	{
		numberOfArms = 1;
		mPoints.push_back(center);
		return;
	}

	float angleBetweenPoints = math::constants::PI_F / numberOfArms;

	Vec2D uVec(0.0f, 1.0f);

	for (unsigned int side = 0; side < numberOfArms; side++)
	{
		mPoints.push_back(center + radius * uVec);
		uVec.Rotate(Vec2D::Zero(), angleBetweenPoints);
		mPoints.push_back(center + thickness * uVec);
		uVec.Rotate(Vec2D::Zero(), angleBetweenPoints);
	}
}