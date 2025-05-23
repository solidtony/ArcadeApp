#pragma once

#include "Utils/Vec2D.h"
#include "Games/Excluder.h"

#include <string>
#include <vector>
#include <random>
#include "PacmanGameUtils.h"
#include "GhostAI.h"
#include "Graphics/BMPImage.h"

class Screen;
class Pacman;
class SpriteSheet;
class Ghost;

class PacmanLevel
{
public:
	bool Init(const std::string& levelPath, const SpriteSheet* noptrSpriteSheet);
	void Update(uint32_t dt, Pacman& pacman, std::vector<Ghost>& ghosts, std::vector<GhostAI>& ghostsAIs);
	void Draw(Screen& screen);

	bool WillCollide(const AARectangle& bbox, PacmanMovement direction) const;
	bool WillCollide(const Ghost& ghost, const GhostAI& ghostAI, PacmanMovement direction) const;
	void ResetLevel();

	inline Vec2D GetLayoutOffset() const { return mLayoutOffset; }
	inline Vec2D GetPacmanSpawnLocation() const { return mPacmanSpawnLocation; }

	bool IsLevelOver() const;
	void IncreaseLevel();
	void ResetToFirstLevel();

	inline const std::vector<Vec2D>& GetGhostSpawnPoints() { return mGhostsSpawnPoints; }

	inline uint32_t GetInGameTextYPos() const { return mBonusItem.bbox.GetTopLeftPoint().GetY(); }

private:
	struct Tile
	{
		Vec2D position = Vec2D::Zero();
		Vec2D offset = Vec2D::Zero();
		int width = 0;
		int collidable = 0;
		int isTeleportTile = 0;
		int excludePelletTile = 0;
		int isPacmanSpawnPoint = 0;
		int isItemSpawnPoint = 0;
		int blinkySpawnPoint = 0;
		int inkySpawnPoint = 0;
		int pinkySpawnPoint = 0;
		int clydeSpawnPoint = 0;
		int isGate = 0;
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

	struct BonusItem
	{
		uint32_t score = 0;
		AARectangle bbox;
		int eaten = 0;
		int spawned = 0;
		int spawnTime = -1;
	};

	struct BonusItemLevelProperties
	{
		uint32_t score = 0;
		std::string spriteName = "";
		uint32_t begin = 0; // which levels does this item spawn at
		uint32_t end = 0;
	};

	bool LoadLevel(const std::string& levelPath);
	Tile* GetTileForSymbol(char symbol);
	void ResetPellets();
	bool HasEatenAllPellets() const;
	size_t NumPelletsEaten() const;
	void GetBonusItemSpriteName(std::string& spriteName, uint32_t& score) const;
	void SpawnBonusItem();
	bool ShouldSpawnBonusItem() const;

	std::default_random_engine mGenerator;
	BonusItem mBonusItem;
	std::string mBonusItemSpriteName;
	const SpriteSheet* mnoptrSpriteSheet;
	std::vector<BonusItemLevelProperties> mBonusItemProperties;
	std::vector<Excluder> mWalls;
	std::vector<Tile> mTiles;

	std::vector<Tile> mExclusionTiles;
	std::vector<Pellet> mPellets;

	std::vector<Excluder> mGate;

	Vec2D mPacmanSpawnLocation;
	Vec2D mLayoutOffset;
	size_t mTileHeight;
	int mCurrentLevel;

	std::vector<Vec2D> mGhostsSpawnPoints;
    BMPImage mBGImage;
};
