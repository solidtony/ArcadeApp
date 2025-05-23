#pragma once

#include <string>

#include "Actor.h"

class Screen;
class SpriteSheet;

class Pacman : public Actor
{
public:
	virtual void Init(const SpriteSheet& spriteSheet, const std::string& animationPath, const Vec2D& initialPos, uint32_t movementSpeed, bool updateSpriteOnMovement);
	virtual void Update(uint32_t dt) override;
	virtual void SetMovementDirection(PacmanMovement movementDir) override;

	void ResetToFirstAnimation();
	void EatenByGhost();
	void ResetScore();
	void AteItem(uint32_t value);
	void AteGhost(uint32_t value);

	inline void ResetGhostEatenMultiplier() { mGhostMultiplier = 1; }
	inline int32_t Score() const { return mScore; }


private:
	void AddToScore(uint32_t value);

	uint32_t mScore;
	uint32_t mGhostMultiplier;
	bool mIsDying; // death animation
};
