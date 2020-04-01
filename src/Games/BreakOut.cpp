#include "Games/BreakOut.h"

#include <iostream>

/*
	Paddle
		- Can move side to side (by the user)
		- Stops at the edge of the screen
		- Width and Height
		- Bounces the ball
			- Bounces the ball differently bassed on the position that was hit on the paddle
				
	Ball
		- Boucnes off of the walls, the paddle, and the blocks
		- Width and height (AARectangles)
		- Position and Velocity

	Block
		- Part of the level
		- Has HP - one hit by the ball is normally -1 HP
		- Color and Position
		- Width and Height (AARectangles)
		- Bounces the ball

	Level
		- Container that holds all of the blocks
		- Should be able to load a level from the levels.txt file
		- Reset the level
		Questions
			- Handle collisions of the blocks?
			- Contain the walls/edges of the screens?

	Game
		- Contains all of the objects (Paddle, Ball, Levels)
		- Player state (number of lives left)
		- States - Serve the ball, In Game, Game Over
		- Reset the game
*/

void BreakOut::Init(GameController& controller)
{
	std::cout << GetName() << " Game Init()" << std::endl;
}

void BreakOut::Update(uint32_t dt)
{
	std::cout << GetName() << " Game Update()" << std::endl;
}

void BreakOut::Draw(Screen& screen)
{
	std::cout << GetName() << " Game Draw()" << std::endl;
}

std::string BreakOut::GetName() const
{
	return GAME_NAME;
}