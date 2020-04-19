#pragma once

#include "Game.h"
#include "Games/Ball.h"
#include "Games/Paddle.h"

class BreakOut : public Game
{
public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;

private:
	void ResetGame();

	Paddle mPaddle;
	Ball mBall;
};