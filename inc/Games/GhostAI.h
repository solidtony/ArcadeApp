#pragma once

#include "Games/Ghost.h"
#include "Games/PacmanGameUtils.h"
#include <random>
#include <stdint.h>

class PacmanLevel;
class Screen;
class Pacman;

enum GhostAIState
{
	START = 0,
	IN_PEN,
	EXIT_PEN,
	CHASE,
	SCATTER,
	GO_TO_PEN
};

class GhostAI
{
public:
	GhostAI();
	void Init(Ghost& ghost, uint32_t lookAheadDistance, const Vec2D& scatterTarget, GhostName name);

	PacmanMovement Update(uint32_t dt, const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost>& ghosts);
	void Draw(Screen& screen);

private:

	void SetState(GhostAIState state);
	void ChangeTarget(const Vec2D& target);
	Vec2D GetChaseTarget(uint32_t dt, const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost>& ghosts);


	Vec2D mScatterTarget;
	Vec2D mTarget;
	uint32_t mLookAheadDistance;
	Ghost* mnoptrGhost;
	GhostName mName;
	std::default_random_engine mAIRandomGenerator;
	GhostAIState mState;
	GhostAIState mLastState;
	uint64_t mTimer;
};