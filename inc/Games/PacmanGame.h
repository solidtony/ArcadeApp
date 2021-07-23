#pragma once

#include "Games/Game.h"
#include "Games/Ghost.h"
#include "Games/PacmanLevel.h"
#include "Games/Pacman.h"
#include "Graphics/SpriteSheet.h"
#include "Input/InputAction.h"
#include "Games/GhostAI.h"

enum PacmanGameState
{
	LEVEL_STARTING = 0,
	PLAY_GAME = 0,
	LOST_LIFE,
	GAME_OVER
};

class PacmanGame: public Game
{
public:
	virtual void Init(GameController& controller) override;
	virtual void Update(uint32_t dt) override;
	virtual void Draw(Screen& screen) override;
	virtual const std::string& GetName() const override;
private:

	void ResetGame();
	void UpdatePacmanMovement();
	void HandleGameControllerState(uint32_t dt, InputState state, PacmanMovement direction);
	void ResetLevel();
	void DrawLives(Screen& screen);
	void SetupGhosts();

	PacmanMovement mPressedDirection;
	SpriteSheet mPacmanSpriteSheet;
	Pacman mPacman;
	PacmanLevel mLevel;
	size_t mNumLives;
	std::vector<Ghost> mGhosts;
	std::vector<GhostAI> mGhostsAIs;
	uint32_t mReleaseGhostTimer;
	PacmanGameState mGameState;
	uint32_t mLevelStartingTimer;
	AARectangle mStringRect;
};