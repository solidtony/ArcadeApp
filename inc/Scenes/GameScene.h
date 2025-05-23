#pragma once

#include <string>
#include <memory>

#include "Games/Game.h"
#include "Scene.h"
 
class GameScene : public Scene
{
public:
	GameScene(std::unique_ptr<Game> optrGame);
	virtual ~GameScene() {}

	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;

	virtual const std::string& GetSceneName() const override;

private:
	std::unique_ptr<Game> mGame;

};