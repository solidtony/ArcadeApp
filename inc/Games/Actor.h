#pragma once

#include "Graphics/AnimatedSprite.h"
#include "Utils/Vec2D.h"
#include "Games/PacmanGameUtils.h"
#include "Shapes/AARectangle.h"

class Screen;

class Actor
{
public:
protected:
	inline void ResetDelta() { mDelta = Vec2D::Zero(); }
	void SetAnimation(const std::string& animationName, bool looped);
	inline void SetMovementSpeed(uint32_t movementSpeed) { mMovementSpeed = movementSpeed; }

private:
	Vec2D mDelta;
	PacmanMovement mMovementDirection;
	uint32_t mMovementSpeed;
	bool mUpdateSpriteOnUpdate;
};