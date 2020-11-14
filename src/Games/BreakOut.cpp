#include "Games/BreakOut.h"

#include <iostream>

#include "App/App.h"

#include "Input/GameController.h"

#include "Shapes/Circle.h"

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

	ButtonAction serveAction;
	serveAction.key = GameController::ActionKey();
	serveAction.action = [this](uint32_t dt, InputState state)
	{
		if (mGameState == IN_SERVE)
		{
			if (GameController::IsPressed(state))
			{
				mGameState = IN_PLAY;

				if (mPaddle.IsMovingLeft())
				{
					mBall.SetVelocity(Vec2D(-INITIAL_BALL_SPEED, -INITIAL_BALL_SPEED));
				}
				else
				{
					mBall.SetVelocity(Vec2D(INITIAL_BALL_SPEED, -INITIAL_BALL_SPEED));
				}
			}
		}
		else if (mGameState == IN_GAME_OVER)
		{
			if (GameController::IsPressed(state))
			{
				ResetGame();
			}
		}
	};

	controller.AddInputActionForKey(serveAction);

	ButtonAction leftKeyAction;
	leftKeyAction.key = GameController::LeftKey();
	leftKeyAction.action = [this](uint32_t dt, InputState state)
	{
		if (mGameState == IN_PLAY || mGameState == IN_SERVE)
		{
			if (GameController::IsPressed(state))
			{
				mPaddle.SetMovementDirection(PaddleDirection::LEFT);
			}
			else
			{
				mPaddle.UnsetMovementDirection(PaddleDirection::LEFT);
			}
		}
	};
	controller.AddInputActionForKey(leftKeyAction);

	ButtonAction RightKeyAction;
	RightKeyAction.key = GameController::RightKey();
	RightKeyAction.action = [this](uint32_t dt, InputState state)
	{
		if (mGameState == IN_PLAY || mGameState == IN_SERVE)
		{
			if (GameController::IsPressed(state))
			{
				mPaddle.SetMovementDirection(PaddleDirection::RIGHT);
			}
			else
			{
				mPaddle.UnsetMovementDirection(PaddleDirection::RIGHT);
			}
		}
	};
	controller.AddInputActionForKey(RightKeyAction);
}

void BreakOut::Update(uint32_t dt)
{
	if (mGameState == IN_SERVE)
	{
		mPaddle.Update(dt, mBall);
		SetToServeState();
	}
	else if (mGameState == IN_PLAY)
	{
		mBall.Update(dt);
		mPaddle.Update(dt, mBall);

		BoundaryEdge edge;

		if (mPaddle.Bounce(mBall))
		{
			return;
		}

		if (mLevelBoundary.HasCollided(mBall, edge))
		{
			mBall.Bounce(edge);
			return;
		}

		GetCurrentLevel().Update(dt, mBall);

		if (IsBallPassedCutoffY())
		{
			ReduceLifeByOne();
			if (!IsGameOver())
			{
				SetToServeState();
			}
			else
			{
				mGameState = IN_GAME_OVER;
			}
		}
		else if (GetCurrentLevel().IsLevelComplete())
		{
			mCurrentLevel = (mCurrentLevel + 1) % mLevels.size();
			ResetGame(mCurrentLevel);
		}
	}
}

void BreakOut::Draw(Screen& screen)
{
	mBall.Draw(screen);
	mPaddle.Draw(screen);
	GetCurrentLevel().Draw(screen);

	screen.Draw(mLevelBoundary.GetAARectangle(), Color::White());

	// Draw cutoff
	Line2D cutoff = { Vec2D(0, mYCutoff), Vec2D(App::Singleton().Width(), mYCutoff) };
	screen.Draw(cutoff, Color::Red());

	// Draw the remaining lives
	Circle lifeCircle = { Vec2D(7, App::Singleton().Height() - 11), 5 };

	for (int i = 0; i < mLives; ++i)
	{
		screen.Draw(lifeCircle, Color::Red(), true, Color::Red());
		lifeCircle.MoveBy(Vec2D(17, 0));
	}
}

const std::string& BreakOut::GetName() const
{
	static std::string name = "Break Out!";

	return name;
}

void BreakOut::ResetGame(size_t toLevel)
{
	mLevels = BreakoutGameLevel::LoadLevelsFromFile(App::GetBasePath() + "Assets\\BreakoutLevels.txt");

	mCurrentLevel = toLevel;
	mLives = NUM_LIVES;

	mYCutoff = App::Singleton().Height() - 2 * Paddle::PADDLE_HEIGHT;

	AARectangle paddleRect = { Vec2D(App::Singleton().Width() / 2.f - Paddle::PADDLE_WIDTH / 2.f, App::Singleton().Height() - 3.f * Paddle::PADDLE_HEIGHT), Paddle::PADDLE_WIDTH, Paddle::PADDLE_HEIGHT };
	AARectangle levelBoundary = { Vec2D::Zero(), Vec2D(App::Singleton().Width()-1.f, App::Singleton().Height()-1.f) };
	mLevelBoundary = {levelBoundary};
	mPaddle.Init(paddleRect, levelBoundary);

	SetToServeState();
}

void BreakOut::SetToServeState()
{
	mGameState = IN_SERVE;
	mBall.Stop();

	mBall.MoveTo(Vec2D(mPaddle.GetAARectangle().GetCenterPoint().GetX(), mPaddle.GetAARectangle().GetTopLeftPoint().GetY() - mBall.GetRadius() - 1.f));
}

bool BreakOut::IsBallPassedCutoffY() const
{
	return mBall.GetPosition().GetY() > mYCutoff;
}

void BreakOut::ReduceLifeByOne()
{
	if (mLives >= 0)
	{
		--mLives;
	}
}