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
		mAnimationPlayer.Init(App::Singleton().GetBasePath() + "Assets/Pacman_animations.txt");
		mSpriteSheet.Load("PacmanSprites");
		mAnimationPlayer.Play("move_right", true);
	}
}

void ArcadeScene::Update(uint32_t dt)
{
	// TESTING
	mAnimationPlayer.Update(dt);
	if (offsetX >= 200)
	{
		mAnimationPlayer.Play("move_left", true);
		travleDir = -1;
	}
	if (offsetX <= 2)
	{
		mAnimationPlayer.Play("move_right", true);
		travleDir = 1;
	}
	offsetX += travleDir*0.5;
}

void ArcadeScene::Draw(Screen& theScreen)
{
	ButtonOptionsScene::Draw(theScreen);

	// TESTING
	AnimationFrame frame = mAnimationPlayer.GetCurrentAnimationFrame();
	frame.offset.SetX(offsetX);
	theScreen.Draw(mSpriteSheet, frame.frame, frame.offset, frame.frameColor);
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