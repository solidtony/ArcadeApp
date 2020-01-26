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
	delete[] mSides;
}

Star::Star(Vec2D center, float radius, float thickness, unsigned int numberOfArms)
	: mCenter(center), mRadius(radius), mThickness(thickness), mNumberOfArms(numberOfArms), mNumberOfSides(2*numberOfArms)
{
	if (mNumberOfArms == 0)
	{
		mNumberOfArms = 1;
		mNumberOfSides = 2 * mNumberOfArms;
	}

	mSides = new Line2D[mNumberOfSides];

	CalculateStar();
}

void Star::CalculateStar()
{
	float angleBetweenArms = 2 * math::constants::PI_F / mNumberOfArms;

	Vec2D uVec(0.0f, 1.0f);

	Vec2D firstArm = mRadius * uVec;

	// Calculate left line
	uVec.Rotate(Vec2D::Zero(), -0.5f * angleBetweenArms);
	mSides[0] = Line2D(mCenter + mThickness * uVec, mCenter + firstArm);

	// Calculate right line
	uVec.Rotate(Vec2D::Zero(), angleBetweenArms);
	mSides[1] = Line2D(mCenter + mThickness * uVec, mCenter + firstArm);

	for (unsigned int side = 2; side < mNumberOfSides; side++)
	{
		mSides[side] = mSides[side - 2].RotationResult(mCenter, angleBetweenArms);
	}
}

void Star::Rotate(Vec2D aroundPoint, float byAngle)
{
	for (unsigned int side = 0; side < mNumberOfSides; ++side)
	{
		mSides[side].Rotate(aroundPoint, byAngle);
	}
}