#include "Shapes/Stars2D.h"

#include "Utils/Utils.h"

Star2D::Star2D() : Star2D(0.0f, 0.0f, 1.0f, 0.2f, 3)
{

}

Star2D::Star2D(float centerX, float centerY, float radius, float thickness, unsigned int numberOfPoints) : Star2D(Vec2D(centerX, centerY), radius, thickness, numberOfPoints)
{

}

Star2D::~Star2D()
{

}

Star2D::Star2D(Vec2D center, float radius, float thickness, unsigned int numberOfPoints) : mCenter(center), mRadius(radius), mThickness(thickness), mNumberOfPoints(numberOfPoints)
{
	mSides = {};

	float angleBetweenPoints = 2*math::constants::PI_F / numberOfPoints;

	Vec2D uVec(0.0f, 1.0f);

	Vec2D point = mRadius*uVec;

	// Calculate left line
	uVec.Rotate(Vec2D::Zero(), -0.5f * angleBetweenPoints);
	mSides.push_back(Line2D(center + mThickness * uVec, center + point));

	// Calculate right point
	uVec.Rotate(Vec2D::Zero(), angleBetweenPoints);
	mSides.push_back(Line2D(center + mThickness * uVec, center + point));

	for (unsigned int side = 2; side < 2*numberOfPoints; side++)
	{
		mSides.push_back(mSides[side - 2].RotationResult(center, angleBetweenPoints));
	}
}

Star2D Star2D::Rotate(Vec2D aroundPoint, float byAngle)
{
	for (auto& side : mSides)
	{
		side.Rotate(aroundPoint, byAngle);
	}

	return *this;
}