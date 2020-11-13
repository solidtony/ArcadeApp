#pragma once

#include "Games/Ball.h"
#include "Games/Game.h"
#include "Games/LevelBoundary.h"
#include "Games/Paddle.h"
#include "Games/BreakoutGameLevel.h"

class BreakOut : public Game
{
public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;

private:
	void ResetGame();

	const Vec2D INITIAL_BALL_VEL = Vec2D(100.f, -100.f);

	Paddle mPaddle;
	Ball mBall;
	LevelBoundary mLevelBoundary;
	BreakoutGameLevel mLevel;
};