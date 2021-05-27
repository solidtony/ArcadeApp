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

	mLevel.Init(App::Singleton().GetBasePath() + "Assets/Pacman_level.txt", &mPacmanSpriteSheet, &mPacman);

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

	mLevel.Update(dt);

	if (mLevel.IsLevelOver())
	{
		mLevel.IncreaseLevel();
	}
}

void PacmanGame::Draw(Screen& screen)
{
	mLevel.Draw(screen);
	mPacman.Draw(screen);

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