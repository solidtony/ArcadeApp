#pragma once

#include "Game.h"

class BreakOut : public Game
{
public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual std::string GetName() const override;

private:
	static constexpr char GAME_NAME[] = "Break Out";
};