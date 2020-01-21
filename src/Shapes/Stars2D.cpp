#include "Shapes/Stars2D.h"

#include "Utils/Utils.h"

Star2D::Star2D() : Star2D(0.0f, 0.0f, 1.0f, 0.2f, 3)
{

}

Star2D::Star2D(float centerX, float centerY, float radius, float thickness, unsigned int numberOfArms) : Star2D(Vec2D(centerX, centerY), radius, thickness, numberOfArms)
{

}

Star2D::~Star2D()
{
	delete[] mSides;
}

Star2D::Star2D(Vec2D center, float radius, float thickness, unsigned int numberOfArms)
	: mCenter(center), mRadius(radius), mThickness(thickness), mNumberOfArms(numberOfArms), mNumberOfSides(2*numberOfArms)
{
	mSides = new Line2D[mNumberOfSides];

	float angleBetweenArms = 2*math::constants::PI_F / numberOfArms;

	Vec2D uVec(0.0f, 1.0f);

	Vec2D firstArm = mRadius*uVec;

	// Calculate left line
	uVec.Rotate(Vec2D::Zero(), -0.5f * angleBetweenArms);
	mSides[0] = Line2D(center + mThickness * uVec, center + firstArm);

	// Calculate right line
	uVec.Rotate(Vec2D::Zero(), angleBetweenArms);
	mSides[1] = Line2D(center + mThickness * uVec, center + firstArm);

	for (unsigned int side = 2; side < mNumberOfSides; side++)
	{
		mSides[side] = mSides[side - 2].RotationResult(center, angleBetweenArms);
	}
}

void Star2D::Rotate(Vec2D aroundPoint, float byAngle)
{
	for (unsigned int side = 0; side < mNumberOfSides; ++side)
	{
		mSides[side].Rotate(aroundPoint, byAngle);
	}
}