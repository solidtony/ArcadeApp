#pragma once

#include "Scene.h"
#include <memory>
#include "Graphics/SpriteSheet.h" // TODO REMOVE after testing

enum eGame
{
	TETRIS = 0,
	BREAK_OUT,
	ASTEROIDS,
	PACMAN,
	NUM_GAMES
};

class Screen;

class ArcadeScene : public Scene
{
public:
	ArcadeScene();
	virtual void Init() override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& theScreen) override;
	virtual const std::string& GetSceneName() const override;

private:
	std::unique_ptr<Scene> GetScene(eGame game);

	// TODO REMOVE after testing
	SpriteSheet mTempSS;
};