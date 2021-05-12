#pragma once

#include "Utils/Vec2d.h"
#include "Games/Excluder.h"

#include <string>
#include <vector>
#include "PacmanGameUtils.h"

class Screen;
class Pacman;

class PacmanLevel
{
public:
	bool Init(const std::string& levelPath, Pacman* noptrPacman);
	void Update(uint32_t dt);
	void Draw(Screen& screen);

	bool WillCollide(const AARectangle& bbox, PacmanMovement direction) const;

	inline Vec2D GetLayoutOffset() const { return mLayoutOffset; }

private:
	struct Tile
	{
		Vec2D position = Vec2D::Zero();
		Vec2D offset = Vec2D::Zero();
		int width = 0;
		int collidable = 0;
		int isTeleportTile = 0;
		int excludePelletTile = 0;
		char teleportToSymbol = 0;
		char symbol = '-';
	};

	struct Pellet
	{
		uint32_t score = 0;
		AARectangle mBBox;
		int isPowerPellet = 0;
		int isEaten = 0;
	};

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);
	void ResetPellets();
	void ResetLevel();

	std::vector<Excluder> mWalls;
	std::vector<Tile> mTiles;

	std::vector<Tile> mExclusionTiles;
	std::vector<Pellet> mPellets;

	Vec2D mLayoutOffset;
	size_t mTileHeight;

	Pacman* mnoptrPacman;
};