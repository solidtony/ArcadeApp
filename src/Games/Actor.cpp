#include <cmath>
#include <string>

#include "Games/Actor.h"
#include "Utils/Utils.h"


void Actor::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Color& spriteColor)
{
	mMovementDirection = PacmanMovement::NONE;
	mSprite.Init(animationsPath, spriteSheet, spriteColor);
	mUpdateSpriteOnUpdate = updateSpriteOnMovement;
	mMovementSpeed = movementSpeed;
	mDelta = Vec2D::Zero();
	mSprite.SetPosition(initialPos);
}

void Actor::Update(uint32_t dt)
{
	if (mMovementDirection != PacmanMovement::NONE)
	{
		Vec2D delta = Vec2D::Zero();

		delta = GetMovementVector(mMovementDirection) * mMovementSpeed;
		mDelta += delta * MillisecondsToSeconds(dt);

		if (fabsf(mDelta.GetX()) >= 1.)
		{
			int dx = int(fabsf(mDelta.GetX()));
			if (mDelta.GetX() < 0.)
			{
				mSprite.MoveBy(Vec2D(-dx, 0.));
				mDelta.SetX(mDelta.GetX() + dx);
			}
			else
			{
				mSprite.MoveBy(Vec2D(dx, 0.));
				mDelta.SetX(mDelta.GetX() - dx);
			}
		}
		else if (fabsf(mDelta.GetY()) >= 1.)
		{
			int dy = int(fabsf(mDelta.GetY()));
			if (mDelta.GetY() < 0.)
			{
				mSprite.MoveBy(Vec2D(0., -dy));
				mDelta.SetY(mDelta.GetY() + dy);
			}
			else
			{
				mSprite.MoveBy(Vec2D(0., dy));
				mDelta.SetY(mDelta.GetY() - dy);
			}
		}

		mSprite.Update(dt);
	}

	if (mUpdateSpriteOnUpdate && mMovementDirection == PacmanMovement::NONE)
	{
		mSprite.Update(dt);
	}
}

void Actor::Draw(Screen& screen)
{
	mSprite.Draw(screen);
}

void Actor::Stop()
{
	SetMovementDirection(PacmanMovement::NONE);
	mSprite.Stop();
}

AARectangle Actor::GetEatingBoundingBox() const
{
	return AARectangle::Inset(GetBoundingBox(), Vec2D{ 3.0, 3.0 });
}

void Actor::SetAnimation(const std::string& animationName, bool looped)
{
	mSprite.SetAnimation(animationName, looped);
}