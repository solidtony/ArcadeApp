#include "Scenes/PacmanStartScene.h"

#include <string>
#include "App/App.h"
#include "Scenes/NotImplementedScene.h"
#include "Scenes/GameScene.h"
#include "Games/PacmanGame.h"
#include <vector>
#include <memory>

PacmanStartScene::PacmanStartScene() :ButtonOptionsScene({"Play Game", "High Scores"}, Color::Yellow())
{

}

void PacmanStartScene::Init()
{
	ButtonAction backAction;
	backAction.key = GameController::CancelKey();
	backAction.action = [](uint32_t dt, InputState state)
	{
		if (GameController::IsPressed(state))
		{
			App::Singleton().PopScene();
		}
	};

	mGameController.AddInputActionForKey(backAction);

	std::vector<Button::ButtonAction> actions;

	actions.push_back([this]() {
		auto pacmanGame = std::make_unique<PacmanGame>();
		App::Singleton().PushScene(std::make_unique<GameScene>(std::move(pacmanGame)));
	});

	actions.push_back([this]() {
		App::Singleton().PushScene(std::make_unique<NotImplementedScene>());
	});

	SetButtonActions(actions);

	ButtonOptionsScene::Init();
}

void PacmanStartScene::Update(uint32_t dt)
{

}

void PacmanStartScene::Draw(Screen& theScreen)
{
	ButtonOptionsScene::Draw(theScreen);
}
const std::string& PacmanStartScene::GetSceneName() const
{
	static std::string name = "!Pacman Start!";
	return name;
}