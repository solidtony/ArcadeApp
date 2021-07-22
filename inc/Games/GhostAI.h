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

class GhostAI : public GhostDelegate
{
public:
	GhostAI();
	void Init(Ghost& ghost, uint32_t lookAheadDistance, const Vec2D& scatterTarget, const Vec2D& ghostPenTarget, const Vec2D& ghostExitPenPosition, GhostName name);

	PacmanMovement Update(uint32_t dt, const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost>& ghosts);
	void Draw(Screen& screen);

	inline bool WantsToLeavePen() const { return mState == GhostAIState::EXIT_PEN; }
	inline bool IsInPen() const { return mState == GhostAIState::IN_PEN || mState == GhostAIState::START; }
	inline bool IsEnteringPen() const { return mState == GhostAIState::GO_TO_PEN; }

	virtual void GhostDelegateGhostStateChangedTo(GhostState lastState, GhostState state) override;
	virtual void GhostWasReleasedFromPen() override;
	virtual void GhostWasResetToFirstPosition() override;

private:

	void SetState(GhostAIState state);
	void ChangeTarget(const Vec2D& target);
	Vec2D GetChaseTarget(uint32_t dt, const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost>& ghosts);

	Vec2D mGhostExitPenPosition;
	Vec2D mGhostPenTarget;
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