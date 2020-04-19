#pragma once

#include <stdint.h>
#include <string>

class GameController;
class Screen;

// Game Interface
class Game
{
public:
	virtual ~Game(){}
	virtual void Init(GameController& controller) = 0;
	virtual void Update(uint32_t dt) = 0;
	virtual void Draw(Screen& screen) = 0;
	virtual const std::string& GetName() const = 0;
};