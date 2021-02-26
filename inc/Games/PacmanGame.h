#pragma once

#include "Games/Game.h"
#include "Games/PacmanLevel.h"

class PacmanGame: public Game
{
public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;
private:
	PacmanLevel mLevel;
};