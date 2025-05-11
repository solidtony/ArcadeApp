#include "Games/PacmanGame.h"

#include <string>

#include "Games/PacmanGameUtils.h"

#include "App/App.h"
#include "Input/GameController.h"

namespace
{
	const std::string SCORE_STR = "Score  ";
	const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
	const size_t MAX_NUM_LIVES = 3;
	const uint32_t RELEASE_GHOST_TIME = 5000;
	const std::string READY_STR = "Ready!";
	const std::string GAME_OVER_STR = "Game Over";
	const uint32_t LEVEL_STARTING_TIME = 300;
}

void PacmanGame::Init(GameController& controller)
{
	mReleaseGhostTimer = 0;
	mPacmanSpriteSheet.Load("PacmanSprites");
	mPacman.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "res/Pacman_animations.txt", Vec2D::Zero(), PACMAN_MOVEMENT_SPEED, false);

	mLevel.Init(App::Singleton().GetBasePath() + "res/Pacman_level.txt", &mPacmanSpriteSheet);

	mStringRect = AARectangle(Vec2D(0, mLevel.GetInGameTextYPos()), App::Singleton().Width(), mPacman.GetBoundingBox().GetHeight());

	SetupGhosts();
	ResetGame();

	ButtonAction leftAction;
	leftAction.key = GameController::LeftKey();
	leftAction.action = [this](uint32_t dt, InputState state)
		{
			HandleGameControllerState(dt, state, PacmanMovement::LEFT);
		};
	controller.AddInputActionForKey(leftAction);

	ButtonAction rightAction;
	rightAction.key = GameController::RightKey();
	rightAction.action = [this](uint32_t dt, InputState state)
	{
		HandleGameControllerState(dt, state, PacmanMovement::RIGHT);
	};
	controller.AddInputActionForKey(rightAction);

	ButtonAction upAction;
	upAction.key = GameController::UpKey();
	upAction.action = [this](uint32_t dt, InputState state)
	{
		HandleGameControllerState(dt, state, PacmanMovement::UP);
	};
	controller.AddInputActionForKey(upAction);

	ButtonAction downAction;
	downAction.key = GameController::DownKey();
	downAction.action = [this](uint32_t dt, InputState state)
	{
		HandleGameControllerState(dt, state, PacmanMovement::DOWN);
	};
	controller.AddInputActionForKey(downAction);
}

void PacmanGame::Update(uint32_t dt)
{
	if (mGameState == PacmanGameState::LEVEL_STARTING)
	{
		mLevelStartingTimer += dt;

		if (mLevelStartingTimer >= LEVEL_STARTING_TIME)
		{
			mLevelStartingTimer = 0;
			mGameState = PacmanGameState::PLAY_GAME;
		}
	}
	else if (mGameState == PacmanGameState::PLAY_GAME)
	{
		UpdatePacmanMovement();
		mPacman.Update(dt);

		mReleaseGhostTimer += dt;

		for (size_t i = 0; i < GhostName::NUM_GHOSTS; ++i)
		{

			Ghost& ghost = mGhosts[i];
			GhostAI& ghostAI = mGhostsAIs[i];

			if (mReleaseGhostTimer >= RELEASE_GHOST_TIME && ghostAI.IsInPen() && !ghost.IsReleased())
			{
				mReleaseGhostTimer = 0;
				ghost.ReleaseFromPen();
			}

			auto direction = ghostAI.Update(dt, mPacman, mLevel, mGhosts);
			if ((ghost.IsReleased() && ghost.CanChangeDirection()) || (ghost.IsReleased() && ghostAI.WantsToLeavePen() && direction != PacmanMovement::NONE))
			{
				if (direction != ghost.GetMovementDirection())
				{
					ghost.SetMovementDirection(direction);
					ghost.LockCanChangeDirection();
				}
			}

			ghost.Update(dt);
		}

		mLevel.Update(dt, mPacman, mGhosts, mGhostsAIs);

		if (mLevel.IsLevelOver())
		{
			mLevel.IncreaseLevel();
			ResetLevel();
		}
	}
	else if (mGameState == PacmanGameState::LOST_LIFE)
	{
		mPacman.Update(dt);

		if (mPacman.IsFinishedAnimation())
		{
			if (mNumLives >= 0)
			{
				ResetLevel();
			}
			else
			{
				mGameState = PacmanGameState::GAME_OVER;
			}
		}
	}
}

void PacmanGame::Draw(Screen& screen)
{
	mLevel.Draw(screen);
	mPacman.Draw(screen);

	for (auto& ghost : mGhosts)
	{
		ghost.Draw(screen);
	}

	for (auto& ghostAI : mGhostsAIs)
	{
		ghostAI.Draw(screen);
	}

	const auto& font = App::Singleton().GetFont();
	Vec2D textDrawPosition;

	// Draw Score
	{
		Vec2D levelOffset = mLevel.GetLayoutOffset();

		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::Singleton().Width(), levelOffset.GetY());

		std::string scoreStr = std::to_string(mPacman.Score());

		textDrawPosition = font.GetDrawPosition(SCORE_STR + scoreStr, highScoreRect, BFXA_CENTER, BFYA_CENTER);

		screen.Draw(font, SCORE_STR + scoreStr, textDrawPosition);
	}

	if (mGameState == PacmanGameState::LEVEL_STARTING)
	{
		textDrawPosition = font.GetDrawPosition(READY_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);

		screen.Draw(font, READY_STR, textDrawPosition, Color::Yellow());
	}
	else if (mGameState == PacmanGameState::GAME_OVER)
	{
		textDrawPosition = font.GetDrawPosition(GAME_OVER_STR, mStringRect, BFXA_CENTER, BFYA_CENTER);
		screen.Draw(font, GAME_OVER_STR, textDrawPosition, Color::Yellow());
	}


	DrawLives(screen);
}

void PacmanGame::DrawLives(Screen& screen)
{
	const uint32_t X_PAD = 1;

	auto sprite = mPacmanSpriteSheet.GetSprite(PACMAN_LIFE_SPRITE_NAME);

	uint32_t xPos = X_PAD;

	for (int i = 0; i < mNumLives; ++i)
	{
		screen.Draw(mPacmanSpriteSheet, PACMAN_LIFE_SPRITE_NAME, Vec2D(xPos, App::Singleton().Height() - sprite.height));
		xPos += X_PAD + sprite.width;
	}
}

const std::string& PacmanGame::GetName() const
{
	static std::string name = "Level";
	return name;
}

void PacmanGame::ResetGame()
{
	mNumLives = MAX_NUM_LIVES;
	mPressedDirection = PacmanMovement::NONE;
	mPacman.ResetScore();
	mLevel.ResetToFirstLevel();
	ResetLevel();
}

void PacmanGame::ResetLevel()
{
	mGameState = PacmanGameState::LEVEL_STARTING;
	mPacman.MoveTo(mLevel.GetPacmanSpawnLocation());
	mPacman.ResetToFirstAnimation();

	for (auto& ghost : mGhosts)
	{
		ghost.ResetToFirstPostion();
	}

	mGhosts[GhostName::BLINKY].ReleaseFromPen();
}

void PacmanGame::UpdatePacmanMovement()
{
	if (mPressedDirection != PacmanMovement::NONE)
	{
		if (!mLevel.WillCollide(mPacman.GetBoundingBox(), mPressedDirection))
		{
			mPacman.SetMovementDirection(mPressedDirection);
		}
	}
}

void PacmanGame::HandleGameControllerState(uint32_t dt, InputState state, PacmanMovement direction)
{
	if (GameController::IsPressed(state))
	{
		mPressedDirection = direction;
	}
	else if (GameController::IsReleased(state) && mPressedDirection == direction)
	{
		mPressedDirection = PacmanMovement::NONE;
	}
}

void PacmanGame::SetupGhosts()
{
	const Vec2D BLINKY_SCATTER_POS = Vec2D(App::Singleton().Width() - 24, 0);
	const Vec2D INKY_SCATTER_POS = Vec2D(App::Singleton().Width(), App::Singleton().Height());
	const Vec2D PINKY_SCATTER_POS = Vec2D(24, 0);
	const Vec2D CLYDE_SCATTER_POS = Vec2D(0, App::Singleton().Height());

	mGhosts.resize(NUM_GHOSTS);
	mGhostsAIs.resize(NUM_GHOSTS);

	Ghost blinky;
	blinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "res/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[BLINKY], GHOST_MOVEMENT_SPEED, true, Color::Red());
	blinky.SetMovementDirection(PacmanMovement::LEFT);
	mGhosts[BLINKY] = blinky;

	auto blinkyAI = GhostAI();
	blinkyAI.Init(mGhosts[BLINKY], blinky.GetBoundingBox().GetWidth(), BLINKY_SCATTER_POS, mLevel.GetGhostSpawnPoints()[PINKY], mLevel.GetGhostSpawnPoints()[BLINKY], BLINKY);

	mGhostsAIs[BLINKY] = blinkyAI;

	Ghost pinky;
	pinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "res/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[PINKY], GHOST_MOVEMENT_SPEED, true, Color::Pink());
	pinky.SetMovementDirection(PacmanMovement::DOWN);
	mGhosts[PINKY] = pinky;

	auto pinkyAI = GhostAI();
	pinkyAI.Init(mGhosts[PINKY], pinky.GetBoundingBox().GetWidth(), PINKY_SCATTER_POS, mLevel.GetGhostSpawnPoints()[PINKY], mLevel.GetGhostSpawnPoints()[BLINKY], PINKY);

	mGhostsAIs[PINKY] = pinkyAI;

	Ghost inky;
	inky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "res/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[INKY], GHOST_MOVEMENT_SPEED, true, Color::Cyan());
	inky.SetMovementDirection(PacmanMovement::UP);
	mGhosts[INKY] = inky;

	auto inkyAI = GhostAI();
	inkyAI.Init(mGhosts[INKY], inky.GetBoundingBox().GetWidth(), INKY_SCATTER_POS, mLevel.GetGhostSpawnPoints()[PINKY], mLevel.GetGhostSpawnPoints()[BLINKY], INKY);

	mGhostsAIs[INKY] = inkyAI;

	Ghost clyde;
	clyde.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "res/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[CLYDE], GHOST_MOVEMENT_SPEED, true, Color::Orange());
	clyde.SetMovementDirection(PacmanMovement::UP);
	mGhosts[CLYDE] = clyde;

	auto clydeAI = GhostAI();
	clydeAI.Init(mGhosts[CLYDE], clyde.GetBoundingBox().GetWidth(), CLYDE_SCATTER_POS, mLevel.GetGhostSpawnPoints()[PINKY], mLevel.GetGhostSpawnPoints()[BLINKY], CLYDE);

	mGhostsAIs[CLYDE] = clydeAI;

	for (size_t i = 0; i < NUM_GHOSTS; ++i)
	{
		mGhosts[i].SetGhostDelegate(mGhostsAIs[i]);
	}
}
