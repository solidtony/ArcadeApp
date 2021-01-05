#include "Scenes/ArcadeScene.h"

#include <iostream>

#include "Input/GameController.h"

#include "Graphics/Color.h"
#include "Graphics/Screen.h"

#include "App/App.h" // TODO REMOVE after testing

#include "Scenes/GameScene.h"
#include "Scenes/NotImplementedScene.h"
#include "Games/BreakOut.h"

ArcadeScene::ArcadeScene() :ButtonOptionsScene({ "Tetris" , "Break Out!", "Asteroids", "!Pac-man" }, Color::Cyan())
{
	
}

void ArcadeScene::Init()
{
	std::vector<Button::ButtonAction> actions;

	actions.push_back([this]{
		App::Singleton().PushScene(GetScene(TETRIS));
	});

	actions.push_back([this]{
		App::Singleton().PushScene(GetScene(BREAK_OUT));
	});

	actions.push_back([this]
	{
		App::Singleton().PushScene(GetScene(ASTEROIDS));
	});

	actions.push_back([this]{
		App::Singleton().PushScene(GetScene(PACMAN));
	});

	SetButtonActions(actions);

	ButtonOptionsScene::Init();

	// TESTING
	{
		mSpriteSheet.Load("PacmanSprites");
		mSprite.Init(App::Singleton().GetBasePath() + "Assets/Pacman_animations.txt", mSpriteSheet);
		mSprite.SetAnimation("move_right", true);
	}
}

void ArcadeScene::Update(uint32_t dt)
{
	// TESTING
	mSprite.Update(dt);
	auto position = mSprite.Position();
	if (position.GetX() >= 200)
	{
		mSprite.SetAnimation("move_left", true);
		travleDir = -1;
	}
	if (position.GetX() <= 2)
	{
		mSprite.SetAnimation("move_right", true);
		travleDir = 1;
	}
	mSprite.MoveBy({ travleDir * 0.5f, 0.0f });
}

void ArcadeScene::Draw(Screen& theScreen)
{
	ButtonOptionsScene::Draw(theScreen);

	// TESTING
	mSprite.Draw(theScreen);
}

const std::string& ArcadeScene::GetSceneName() const
{
	static std::string sceneName = "Arcade Scene";
	return sceneName; // TODO complete later
}

std::unique_ptr<Scene> ArcadeScene::GetScene(eGame game)
{
	switch (game)
	{
		case TETRIS:
		{

		}
		break;

		case BREAK_OUT:
		{
			std::unique_ptr<BreakOut> breakoutGame = std::make_unique<BreakOut>();
			std::unique_ptr<GameScene> breakoutScene = std::make_unique<GameScene>(std::move(breakoutGame));

			return breakoutScene;
		}
		break;

		case ASTEROIDS:
		{

		}
		break;

		case PACMAN:
		{

		}
		break;

	}

	std::unique_ptr<Scene> notImplementedScene = std::make_unique<NotImplementedScene>();

	return notImplementedScene;
}