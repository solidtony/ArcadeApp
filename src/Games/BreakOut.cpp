#include "Games/BreakOut.h"

#include <iostream>

#include "App/App.h"

#include "Input/GameController.h"

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
	controller.ClearAll();
	ResetGame();

	ButtonAction leftKeyAction;
	leftKeyAction.key = GameController::LeftKey();
	leftKeyAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			mPaddle.SetMovementDirection(PaddleDirection::LEFT);
		}
		else
		{
			mPaddle.UnsetMovementDirection(PaddleDirection::LEFT);
		}
	};
	controller.AddInputActionForKey(leftKeyAction);

	ButtonAction RightKeyAction;
	RightKeyAction.key = GameController::RightKey();
	RightKeyAction.action = [this](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			mPaddle.SetMovementDirection(PaddleDirection::RIGHT);
		}
		else
		{
			mPaddle.UnsetMovementDirection(PaddleDirection::RIGHT);
		}
	};
	controller.AddInputActionForKey(RightKeyAction);
}

void BreakOut::Update(uint32_t dt)
{
	mPaddle.Update(dt);
}

void BreakOut::Draw(Screen& screen)
{
	mPaddle.Draw(screen);
}

const std::string& BreakOut::GetName() const
{
	static std::string name = "Break Out!";

	return name;
}

void BreakOut::ResetGame()
{
	AARectangle paddleRect = { Vec2D(App::Singleton().Width() / 2 - Paddle::PADDLE_WIDTH / 2, App::Singleton().Height() - 3 * Paddle::PADDLE_HEIGHT), Paddle::PADDLE_WIDTH, Paddle::PADDLE_HEIGHT };
	AARectangle levelBoundary = { Vec2D::Zero(), Vec2D(App::Singleton().Width(), App::Singleton().Height()) };

	mPaddle.Init(paddleRect, levelBoundary);
}