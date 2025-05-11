#include "Games/Ghost.h"

#include <string>

namespace
{
	const uint32_t NUM_POINTS_FOR_GHOST = 200;
}

Ghost::Ghost(): mPoints(0), mInitialPos(Vec2D::Zero()), mIsReleased(false), mDeleagte(nullptr)
{

}

void Ghost::Init(const SpriteSheet& spriteSheet, const std::string& animationsPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement, const Color& spriteColor)
{
	Actor::Init(spriteSheet, animationsPath, initialPos, movementSpeed, updateSpriteOnMovement, spriteColor);

	mInitialPos = initialPos;
	mPoints = NUM_POINTS_FOR_GHOST;
	ResetToFirstPostion();
}

void Ghost::Update(uint32_t dt)
{
	Vec2D position = Position();

	Actor::Update(dt);
	mCanChangeDirection = position != Position();

	if (IsVulnerable())
	{
		mVulnerabilityTimer += dt;

		if (mState == GhostState::VULNERABLE && mVulnerabilityTimer >= VULNERABILITY_TIME)
		{
			SetGhostState(GhostState::VULNERABLE_ENDING); // resets the vulnerability timer back to zero
		}

		if (mState == GhostState::VULNERABLE_ENDING && mVulnerabilityTimer >= VULNERABILITY_ENDING_TIME)
		{
			SetGhostState(GhostState::ALIVE);
		}
	}
}

void Ghost::SetStateToVulnerable()
{
	if (mState != GhostState::DEAD)
	{
		SetGhostState(GhostState::VULNERABLE);
	}
}

void Ghost::SetMovementDirection(PacmanMovement direction)
{
	Actor::SetMovementDirection(direction);

	PacmanMovement movementDir = GetMovementDirection();

	if (mState == GhostState::ALIVE)
	{
		if (movementDir == PacmanMovement::RIGHT)
		{
			SetAnimation("ghost_movement_right", true);
		}
		else if (movementDir == PacmanMovement::UP)
		{
			SetAnimation("ghost_movement_up", true);
		}
		else if (movementDir == PacmanMovement::DOWN)
		{
			SetAnimation("ghost_movement_down", true);
		}
		else if (movementDir == PacmanMovement::LEFT)
		{
			SetAnimation("ghost_movement_left", true);
		}
	}
	else if (mState == GhostState::DEAD)
	{
		if (movementDir == PacmanMovement::RIGHT)
		{
			SetAnimation("ghost_dead_right", true);
		}
		else if (movementDir == PacmanMovement::UP)
		{
			SetAnimation("ghost_dead_up", true);
		}
		else if (movementDir == PacmanMovement::DOWN)
		{
			SetAnimation("ghost_dead_down", true);
		}
		else if (movementDir == PacmanMovement::LEFT)
		{
			SetAnimation("ghost_dead_left", true);
		}
	}
}

void Ghost::Stop()
{
	SetMovementDirection(PacmanMovement::NONE);
}

void Ghost::EatenByPacman()
{
	SetGhostState(GhostState::DEAD);
}

void Ghost::ResetToFirstPostion()
{
	mSprite.SetPosition(mInitialPos);
	SetMovementDirection(PacmanMovement::NONE);
	mVulnerabilityTimer = 0;
	SetGhostState(GhostState::ALIVE);
	mCanChangeDirection = true;
	mIsReleased = false;
	if (mDeleagte != nullptr)
	{
		mDeleagte->GhostWasResetToFirstPosition();
	}
}

void Ghost::SetGhostState(GhostState state)
{
	if (mDeleagte)
	{
		mDeleagte->GhostDelegateGhostStateChangedTo(mState, state);
	}

	mState = state;
	switch (mState)
	{
	case ALIVE:
		SetMovementDirection(GetMovementDirection());
		SetMovementSpeed(GHOST_MOVEMENT_SPEED);
		break;
	case VULNERABLE:
		mSprite.SetAnimation("ghost_vulnerable", true);
		mVulnerabilityTimer = 0;
		SetMovementSpeed(GHOST_VULNERABLE_MOVEMENT_SPEED);
		break;
	case VULNERABLE_ENDING:
		mSprite.SetAnimation("ghost_vulnerable_ending", true);
		mVulnerabilityTimer = 0;
		break;
	case DEAD:
		SetMovementDirection(GetMovementDirection());
		SetMovementSpeed(GHOST_BACK_TO_PEN_SPEED);
		break;
	}
}

void Ghost::ReleaseFromPen()
{
	mIsReleased = true;

	if (mDeleagte)
	{
		mDeleagte->GhostWasReleasedFromPen();
	}
}
