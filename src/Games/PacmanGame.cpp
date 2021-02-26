#include "Games/PacmanGame.h"

#include "App/App.h"

void PacmanGame::Init(GameController& controller)
{
	mLevel.Init(App::Singleton().GetBasePath() + "Assets/Pacman_level.txt");
}

void PacmanGame::Update(uint32_t dt)
{
	mLevel.Update(dt);
}

void PacmanGame::Draw(Screen& screen)
{
	mLevel.Draw(screen);
}

const std::string& PacmanGame::GetName() const
{
	static std::string name = "Level";
	return name;
}