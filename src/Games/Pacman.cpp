#include "Games/Pacman.h"

#include <string>

#include "Utils/Utils.h"

void Pacman::Init(const SpriteSheet& spriteSheet, const std::string& animationPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement)
{
	Actor::Init(spriteSheet, animationPath, initialPos, movementSpeed, false);

	ResetToFirstAnimation();
	ResetScore();
	mIsDying = false;
	ResetGhostEatenMultiplier();
}

void Pacman::Update(uint32_t dt)
{
	if (mIsDying)
	{
		mSprite.Update(dt);
		mIsDying = !IsFinishedAnimation();
		return;
	}

	Actor::Update(dt); // base class update
}

void Pacman::SetMovementDirection(PacmanMovement movementDir)
{
	PacmanMovement currentDirection = GetMovementDirection();
	if (movementDir == PacmanMovement::LEFT && currentDirection != PacmanMovement::LEFT)
	{
		SetAnimation("move_left", true);
		ResetDelta();
	}
	else if (movementDir == PacmanMovement::RIGHT && currentDirection != PacmanMovement::RIGHT)
	{
		SetAnimation("move_right", true);
		ResetDelta();
	}
	else if (movementDir == PacmanMovement::DOWN && currentDirection != PacmanMovement::DOWN)
	{
		SetAnimation("move_down", true);
		ResetDelta();
	}
	else if (movementDir == PacmanMovement::UP && currentDirection != PacmanMovement::UP)
	{
		SetAnimation("move_up", true);
		ResetDelta();
	}

	Actor::SetMovementDirection(movementDir);
}

void Pacman::ResetToFirstAnimation()
{
	SetAnimation("move_left", true);
	Stop();
}

void Pacman::EatenByGhost()
{
	SetAnimation("death", true);
	mIsDying = true;
	ResetGhostEatenMultiplier();
}

void Pacman::ResetScore()
{
	mScore = 0;
}

void Pacman::AteItem(uint32_t value)
{
	AddToScore(value);
}

void Pacman::AteGhost(uint32_t value)
{
	AddToScore(value * mGhostMultiplier);
	mGhostMultiplier *= 2;
}

void Pacman::AddToScore(uint32_t value)
{
	mScore += value;
}