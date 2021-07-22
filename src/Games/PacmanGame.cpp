#include "Games/PacmanGame.h"
#include "Games/PacmanGameUtils.h"

#include "App/App.h"
#include "Input/GameController.h"

namespace
{
	const std::string SCORE_STR = "Score  ";
	const std::string PACMAN_LIFE_SPRITE_NAME = "pac_man_left_idle";
	const size_t MAX_NUM_LIVES = 3;
}

void PacmanGame::Init(GameController& controller)
{
	mPacmanSpriteSheet.Load("PacmanSprites");
	mPacman.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Pacman_animations.txt", Vec2D::Zero(), PACMAN_MOVEMENT_SPEED, false);

	mLevel.Init(App::Singleton().GetBasePath() + "Assets/Pacman_level.txt", &mPacmanSpriteSheet);

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
	UpdatePacmanMovement();
	mPacman.Update(dt);

	for (size_t i = 0; i < GhostName::NUM_GHOSTS; ++i)
	{
		if (static_cast<GhostName>(i) == GhostName::BLINKY)
		{
			GhostAI& ghostAI = mGhostsAIs[i];

			auto direction = ghostAI.Update(dt, mPacman, mLevel, mGhosts);

			if (mGhosts[i].CanChangeDirection() && direction != mGhosts[i].GetMovementDirection())
			{
				mGhosts[i].SetMovementDirection(direction);
			}
		}

		mGhosts[i].Update(dt);
	}

	mLevel.Update(dt, mPacman, mGhosts, mGhostsAIs);

	if (mLevel.IsLevelOver())
	{
		mLevel.IncreaseLevel();
		ResetLevel();
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

	// Draw Score
	{
		Vec2D levelOffset = mLevel.GetLayoutOffset();

		AARectangle highScoreRect = AARectangle(Vec2D(0, 4), App::Singleton().Width(), levelOffset.GetY());

		const auto& font = App::Singleton().GetFont();
		Vec2D textDrawPosition;

		std::string scoreStr = std::to_string(mPacman.Score());

		textDrawPosition = font.GetDrawPosition(SCORE_STR + scoreStr, highScoreRect, BFXA_CENTER, BFYA_CENTER);

		screen.Draw(font, SCORE_STR + scoreStr, textDrawPosition);
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
	mPacman.MoveTo(mLevel.GetPacmanSpawnLocation());
	mPacman.ResetToFirstAnimation();
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

	// Blinky
	Ghost blinky;
	blinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[GhostName::BLINKY], GHOST_MOVEMNET_SPEED, true, Color::Red());
	blinky.SetMovementDirection(PacmanMovement::RIGHT);
	mGhosts[GhostName::BLINKY] = blinky;

	auto blinkyAI = GhostAI();
	blinkyAI.Init(mGhosts[GhostName::BLINKY], blinky.GetBoundingBox().GetWidth(), BLINKY_SCATTER_POS, mLevel.GetGhostSpawnPoints()[GhostName::PINKY], mLevel.GetGhostSpawnPoints()[GhostName::BLINKY], GhostName::BLINKY);
	mGhostsAIs[GhostName::BLINKY] = blinkyAI;

	// Pinky
	Ghost pinky;
	pinky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[GhostName::PINKY], GHOST_MOVEMNET_SPEED, true, Color::Pink());
	pinky.SetMovementDirection(PacmanMovement::UP);
	mGhosts[GhostName::PINKY] = pinky;

	auto pinkyAI = GhostAI();
	pinkyAI.Init(mGhosts[GhostName::PINKY], pinky.GetBoundingBox().GetWidth(), PINKY_SCATTER_POS, mLevel.GetGhostSpawnPoints()[GhostName::PINKY], mLevel.GetGhostSpawnPoints()[GhostName::BLINKY], GhostName::PINKY);
	mGhostsAIs[GhostName::PINKY] = pinkyAI;

	// Inky
	Ghost inky;
	inky.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[GhostName::INKY], GHOST_MOVEMNET_SPEED, true, Color::Cyan());
	inky.SetMovementDirection(PacmanMovement::DOWN);
	mGhosts[GhostName::INKY] = inky;

	auto inkyAI = GhostAI();
	inkyAI.Init(mGhosts[GhostName::INKY], inky.GetBoundingBox().GetWidth(), INKY_SCATTER_POS, mLevel.GetGhostSpawnPoints()[GhostName::INKY], mLevel.GetGhostSpawnPoints()[GhostName::BLINKY], GhostName::INKY);
	mGhostsAIs[GhostName::INKY] = inkyAI;

	// Clyde
	Ghost clyde;
	clyde.Init(mPacmanSpriteSheet, App::Singleton().GetBasePath() + "Assets/Ghost_animations.txt", mLevel.GetGhostSpawnPoints()[GhostName::CLYDE], GHOST_MOVEMNET_SPEED, true, Color::Orange());
	clyde.SetMovementDirection(PacmanMovement::RIGHT);
	mGhosts[GhostName::CLYDE] = clyde;

	auto clydeAI = GhostAI();
	clydeAI.Init(mGhosts[GhostName::CLYDE], clyde.GetBoundingBox().GetWidth(), CLYDE_SCATTER_POS, mLevel.GetGhostSpawnPoints()[GhostName::CLYDE], mLevel.GetGhostSpawnPoints()[GhostName::BLINKY], GhostName::CLYDE);
	mGhostsAIs[GhostName::CLYDE] = clydeAI;
}