#include "Games/PacmanLevel.h"

#include <cassert>
#include <string>

#include "App/App.h"
#include "Utils/FileCommandLoader.h"
#include "Graphics/Screen.h"
#include "Graphics/SpriteSheet.h"
#include "Games/Pacman.h"
#include "Games/Ghost.h"
#include "Shapes/Circle.h"
#include "Shapes/AARectangle.h"

namespace {
	const uint32_t NUM_LEVLES = 256;
	const uint32_t SPRITE_HEIGHT = 16;
	const uint32_t SPRITE_WIDTH = 16;
}

bool PacmanLevel::Init(const std::string& levelPath, const SpriteSheet* noptrSpriteSheet)
{
	mCurrentLevel = 0;
	mnoptrSpriteSheet = noptrSpriteSheet;
	mBonusItemSpriteName = "";
	std::random_device r;
	mGenerator.seed(r());
	mGhostsSpawnPoints.resize(GhostName::NUM_GHOSTS);
	bool wasLevelLoaded = LoadLevel(levelPath);

	if (wasLevelLoaded)
	{
		ResetLevel();
	}

	return wasLevelLoaded;
}

void PacmanLevel::Update(uint32_t dt, Pacman& pacman, std::vector<Ghost>& ghosts, std::vector<GhostAI>& ghostsAIs)
{
	for (const auto& wall : mWalls)
	{
		BoundaryEdge edge;

		if (wall.HasCollided(pacman.GetBoundingBox(), edge))
		{
			Vec2D offset = wall.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		for (auto& ghost : ghosts)
		{
			if (wall.HasCollided(ghost.GetBoundingBox(), edge))
			{
				Vec2D offset = wall.GetCollisionOffset(ghost.GetBoundingBox());
				ghost.MoveBy(offset);
				ghost.Stop();
			}
		}
	}
	for (const auto& gate : mGate)
	{
		BoundaryEdge edge;

		if (gate.HasCollided(pacman.GetBoundingBox(), edge))
		{
			Vec2D offset = gate.GetCollisionOffset(pacman.GetBoundingBox());
			pacman.MoveBy(offset);
			pacman.Stop();
		}

		for (size_t i = 0; i < NUM_GHOSTS; ++i)
		{
			GhostAI& ghostAI = ghostsAIs[i];
			Ghost& ghost = ghosts[i];

			if (!(ghostAI.WantsToLeavePen() || ghostAI.IsEnteringPen()) && gate.HasCollided(ghost.GetBoundingBox(), edge))
			{
				Vec2D offset = gate.GetCollisionOffset(ghost.GetBoundingBox());
				ghost.MoveBy(offset);
				ghost.Stop();
			}
		}
	}


	for (Tile t : mTiles)
	{
		if (t.isTeleportTile)
		{
			AARectangle teleportTileAABB(t.position, t.width, static_cast<float>(mTileHeight));

			Tile* teleportToTile = GetTileForSymbol(t.teleportToSymbol);
			assert(teleportToTile);

			if (teleportToTile->isTeleportTile)
			{
				if (teleportTileAABB.Intersects(pacman.GetBoundingBox()))
				{
					pacman.MoveTo(teleportToTile->position + teleportToTile->offset);
				}

				for (auto& ghost : ghosts)
				{
					if (teleportTileAABB.Intersects(ghost.GetBoundingBox()))
					{
						ghost.MoveTo(teleportToTile->position + teleportToTile->offset);
					}
				}
			}
		}
	}

	for (auto& pellet : mPellets)
	{
		if (!pellet.isEaten)
		{
			if (pacman.GetEatingBoundingBox().Intersects(pellet.mBBox))
			{
				pellet.isEaten = true;

				pacman.AteItem(pellet.score);

				if (pellet.isPowerPellet)
				{
					pacman.ResetGhostEatenMultiplier();
                    for (auto& ghost: ghosts)
                    {
                        ghost.SetStateToVulnerable();
                    }
				}
			}
		}
	}

	if (ShouldSpawnBonusItem())
	{
		SpawnBonusItem();
	}

	if (mBonusItem.spawned && !mBonusItem.eaten)
	{
		if (pacman.GetEatingBoundingBox().Intersects(mBonusItem.bbox))
		{
			mBonusItem.eaten = true;
			pacman.AteItem(mBonusItem.score);
		}
	}
}

void PacmanLevel::Draw(Screen& screen)
{
    Sprite bgSprite;
    bgSprite.width = mBGImage.GetWidth();
    bgSprite.height = mBGImage.GetHeight();

    screen.Draw(mBGImage, bgSprite, Vec2D::Zero());

	for (const auto& pellet : mPellets)
	{
		if (!pellet.isEaten)
		{
			if (!pellet.isPowerPellet)
			{
				screen.Draw(pellet.mBBox, Color::White());
			}
			else
			{
				Circle c(pellet.mBBox.GetCenterPoint(), pellet.mBBox.GetWidth() / 2.0f);
				screen.Draw(c, Color::White(), true, Color::White());
			}
		}
	}

	if (mBonusItem.spawned && !mBonusItem.eaten)
	{
		screen.Draw(*mnoptrSpriteSheet, mBonusItemSpriteName, mBonusItem.bbox.GetTopLeftPoint());
	}
}

bool PacmanLevel::WillCollide(const AARectangle& aBBox, PacmanMovement direction) const
{
	AARectangle bbox = aBBox;

	bbox.MoveBy(GetMovementVector(direction));

	BoundaryEdge edge;
	for (const auto& wall : mWalls)
	{
		if(wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	for (const auto& gate : mGate)
	{
		if (gate.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	return false;
}

bool PacmanLevel::WillCollide(const Ghost& ghost, const GhostAI& ghostAI, PacmanMovement direction) const
{
	AARectangle bbox = ghost.GetBoundingBox();

	bbox.MoveBy(GetMovementVector(direction));

	BoundaryEdge edge;
	for (const auto& wall : mWalls)
	{
		if (wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	for (const auto& gate : mGate)
	{
		if (!(ghostAI.IsEnteringPen() || ghostAI.WantsToLeavePen()) && gate.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	return false;
}

bool PacmanLevel::ShouldSpawnBonusItem() const
{
	auto numEaten = NumPelletsEaten();
	return !mBonusItem.spawned && numEaten >= mBonusItem.spawnTime;
}

void PacmanLevel::SpawnBonusItem()
{
	mBonusItem.spawned = 1;
	mBonusItem.eaten = 0;
}

void PacmanLevel::GetBonusItemSpriteName(std::string& spriteName, uint32_t& score) const
{
	for (const auto& properties : mBonusItemProperties)
	{
		if (mCurrentLevel >= properties.begin && mCurrentLevel <= properties.end)
		{
			spriteName = properties.spriteName;
			score = properties.score;
			return;
		}
	}
}

void PacmanLevel::ResetLevel()
{
	ResetPellets();

	std::uniform_int_distribution<size_t> distrubution(20, mPellets.size() - 50);
	mBonusItem.spawnTime = static_cast<int>(distrubution(mGenerator));

	GetBonusItemSpriteName(mBonusItemSpriteName, mBonusItem.score);
}

bool PacmanLevel::IsLevelOver() const
{
	return HasEatenAllPellets();
}

void PacmanLevel::IncreaseLevel()
{
	mCurrentLevel++;

	if (mCurrentLevel > NUM_LEVLES)
	{
		mCurrentLevel = 1;
	}

	ResetLevel();
}

void PacmanLevel::ResetToFirstLevel()
{
	mCurrentLevel = 1;
	ResetLevel();
}

bool PacmanLevel::HasEatenAllPellets() const
{
	return NumPelletsEaten() >= mPellets.size() - 4; // 4 Super pellets
}

size_t PacmanLevel::NumPelletsEaten() const
{
	size_t numEaten = 0;

	for (const auto& pellet : mPellets)
	{
		if (!pellet.isPowerPellet && pellet.isEaten)
		{
			++numEaten;
		}
	}

	return numEaten;
}

void PacmanLevel::ResetPellets()
{
	mPellets.clear();

	const uint32_t PELLET_SIZE = 2;
	const uint32_t PADDING = static_cast<uint32_t>(mTileHeight);

	uint32_t startingY = mLayoutOffset.GetY() + PADDING + mTileHeight - 1;
	uint32_t startingX = PADDING + 3;

	const uint32_t LEVEL_HEIGHT = mLayoutOffset.GetY() + 32 * mTileHeight;

	Pellet p;
	p.score = 10;

	uint32_t row = 0;

	for (uint32_t y = startingY; y < LEVEL_HEIGHT; y += PADDING, ++row)
	{
		for (uint32_t x = startingX, col = 0; x < App::Singleton().Width(); x += PADDING, ++col)
		{
			if (row == 0 || row == 22)
			{
				if (col == 0 || col == 25)
				{
					p.isPowerPellet = 1;
					p.score = 50;
					p.mBBox = AARectangle(Vec2D(x - 3, y - 3), mTileHeight, mTileHeight);
					mPellets.push_back(p);

					p.isPowerPellet = 0;
					p.score = 10;

					continue;
				}
			}

			AARectangle rect = AARectangle(Vec2D(x, y), PELLET_SIZE, PELLET_SIZE);

			bool found = false;

			for (const Excluder& wall : mWalls)
			{
				if (wall.GetAARectangle().Intersects(rect))
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				for (const Tile& excludedPelletTile : mExclusionTiles)
				{
					if (excludedPelletTile.excludePelletTile)
					{
						AARectangle tileAABB(excludedPelletTile.position, excludedPelletTile.width, mTileHeight);
						if (tileAABB.Intersects(rect))
						{
							found = true;
							break;
						}
					}
				}
			}

			if (!found)
			{
				p.mBBox = rect;
				mPellets.push_back(p);
			}
		}
	}
}

bool PacmanLevel::LoadLevel(const std::string& levelPath)
{
	FileCommandLoader fileLoader;

    std::string bgImageName;

	Command bgImageCommand;
	bgImageCommand.command = "bg_image";
	bgImageCommand.parseFunc = [this, &bgImageName](ParseFuncParams params)
	{
        bgImageName = FileCommandLoader::ReadString(params);
        bool loaded = mBGImage.Load(App::Singleton().GetBasePath() + std::string("res/") + bgImageName);

        assert(loaded && "Failed to laod the BG image");
	};
	fileLoader.AddCommand(bgImageCommand);

	Command tileWidthCommand;
	tileWidthCommand.command = "tile_width";
	tileWidthCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().width = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileWidthCommand);

	Command tileHeightCommand;
	tileHeightCommand.command = "tile_height";
	tileHeightCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTileHeight = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileHeightCommand);

	Command tileCommand;
	tileCommand.command = "tile";
	tileCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.push_back(Tile());
	};
	fileLoader.AddCommand(tileCommand);

	Command tileSymbolCommand;
	tileSymbolCommand.command = "tile_symbol";
	tileSymbolCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().symbol = FileCommandLoader::ReadChar(params);
	};
	fileLoader.AddCommand(tileSymbolCommand);

	Command tileCollisionCommand;
	tileCollisionCommand.command = "tile_collision";
	tileCollisionCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().collidable = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileCollisionCommand);

	Vec2D layoutOffset;
	Command layoutOffsetCommand;
	layoutOffsetCommand.command = "layout_offset";
	layoutOffsetCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params)
	{
		mLayoutOffset = FileCommandLoader::ReadSize(params);
		layoutOffset = mLayoutOffset;
	};
	fileLoader.AddCommand(layoutOffsetCommand);

	Command tileIsTeleportTileCommand;
	tileIsTeleportTileCommand.command = "tile_is_teleport_tile";
	tileIsTeleportTileCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isTeleportTile = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileIsTeleportTileCommand);

	Command tileToTeleportToCommand;
	tileToTeleportToCommand.command = "tile_teleport_to_symbol";
	tileToTeleportToCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().teleportToSymbol = FileCommandLoader::ReadChar(params);
	};
	fileLoader.AddCommand(tileToTeleportToCommand);

	Command tileOffsetCommand;
	tileOffsetCommand.command = "tile_offset";
	tileOffsetCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().offset = FileCommandLoader::ReadSize(params);
	};
	fileLoader.AddCommand(tileOffsetCommand);

	Command tileExcludePelletCommand;
	tileExcludePelletCommand.command = "tile_exclude_pellet";
	tileExcludePelletCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().excludePelletTile = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileExcludePelletCommand);

	Command titlePacmanSpawnPointCommand;
	titlePacmanSpawnPointCommand.command = "tile_pacman_spawn_point";
	titlePacmanSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isPacmanSpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(titlePacmanSpawnPointCommand);

	Command titleItemSpawnPoint;
	titleItemSpawnPoint.command = "tile_item_spawn_point";
	titleItemSpawnPoint.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isItemSpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(titleItemSpawnPoint);

	Command tileBlinkySpawnPointCommand;
	tileBlinkySpawnPointCommand.command = "tile_blinky_spawn_point";
	tileBlinkySpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().blinkySpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileBlinkySpawnPointCommand);

	Command tilePinkySpawnPointCommand;
	tilePinkySpawnPointCommand.command = "tile_pinky_spawn_point";
	tilePinkySpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().pinkySpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tilePinkySpawnPointCommand);

	Command tileInkySpawnPointCommand;
	tileInkySpawnPointCommand.command = "tile_inky_spawn_point";
	tileInkySpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().inkySpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileInkySpawnPointCommand);

	Command tileClydeSpawnPointCommand;
	tileClydeSpawnPointCommand.command = "tile_clyde_spawn_point";
	tileClydeSpawnPointCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().clydeSpawnPoint = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileClydeSpawnPointCommand);

	Command tileGateCommand;
	tileGateCommand.command = "tile_is_gate";
	tileGateCommand.parseFunc = [this](ParseFuncParams params)
	{
		mTiles.back().isGate = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(tileGateCommand);

	Command layoutCommand;
	layoutCommand.command = "layout";
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.parseFunc = [&layoutOffset, this](ParseFuncParams params)
	{
		int startingX = layoutOffset.GetX();

		for (size_t c = 0; c < params.line.length(); ++c)
		{
			Tile* tile = GetTileForSymbol(params.line[c]);
			if (tile)
			{
				tile->position = Vec2D(startingX, layoutOffset.GetY());
				if (tile->isGate > 0)
				{
					Excluder gate;

					gate.Init(AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width, static_cast<int>(mTileHeight)));
					mGate.push_back(gate);

				}
				else if (tile->collidable > 0)
				{
					Excluder wall;
					wall.Init(AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width, static_cast<int>(mTileHeight)));
					mWalls.push_back(wall);
				}
				if (tile->isPacmanSpawnPoint > 0)
				{
					mPacmanSpawnLocation = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->isItemSpawnPoint > 0)
				{
					mBonusItem.bbox = AARectangle(Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY()), SPRITE_WIDTH, SPRITE_HEIGHT);
				}
				else if (tile->blinkySpawnPoint > 0)
				{
					mGhostsSpawnPoints[GhostName::BLINKY] = Vec2D(startingX + tile->offset.GetX()+1, layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->pinkySpawnPoint > 0)
				{
					mGhostsSpawnPoints[GhostName::PINKY] = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->inkySpawnPoint > 0)
				{
					mGhostsSpawnPoints[GhostName::INKY] = Vec2D(startingX + tile->offset.GetX()+1, layoutOffset.GetY() + tile->offset.GetY());
				}
				else if (tile->clydeSpawnPoint > 0)
				{
					mGhostsSpawnPoints[GhostName::CLYDE] = Vec2D(startingX + tile->offset.GetX(), layoutOffset.GetY() + tile->offset.GetY());
				}
				if (tile->excludePelletTile > 0)
				{
					mExclusionTiles.push_back(*tile);
				}

				startingX += tile->width;
			}
		}
		layoutOffset += Vec2D(0, mTileHeight);
	};
	fileLoader.AddCommand(layoutCommand);

	Command bonusItemCommand;
	bonusItemCommand.command = "bonus_item";
	bonusItemCommand.parseFunc = [this](ParseFuncParams params)
	{
		BonusItemLevelProperties newProperty;
		mBonusItemProperties.push_back(newProperty);
	};
	fileLoader.AddCommand(bonusItemCommand);

	Command bonusItemSpriteNameCommand;
	bonusItemSpriteNameCommand.command = "bonus_item_sprite_name";
	bonusItemSpriteNameCommand.parseFunc = [this](ParseFuncParams params)
	{
		mBonusItemProperties.back().spriteName = FileCommandLoader::ReadString(params);
	};
	fileLoader.AddCommand(bonusItemSpriteNameCommand);

	Command bonusItemScoreCommand;
	bonusItemScoreCommand.command = "bonus_item_score";
	bonusItemScoreCommand.parseFunc = [this](ParseFuncParams params)
	{
		mBonusItemProperties.back().score = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(bonusItemScoreCommand);

	Command bonusItemBeginLevelCommand;
	bonusItemBeginLevelCommand.command = "bonus_item_begin_level";
	bonusItemBeginLevelCommand.parseFunc = [this](ParseFuncParams params)
	{
		mBonusItemProperties.back().begin = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(bonusItemBeginLevelCommand);

	Command bonusItemEndLevelCommand;
	bonusItemEndLevelCommand.command = "bonus_item_end_level";
	bonusItemEndLevelCommand.parseFunc = [this](ParseFuncParams params)
	{
		mBonusItemProperties.back().end = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(bonusItemEndLevelCommand);

	return fileLoader.LoadFile(levelPath);
}

PacmanLevel::Tile* PacmanLevel::GetTileForSymbol(char symbol)
{
	for (size_t i = 0; i < mTiles.size(); ++i)
	{
		if (mTiles[i].symbol == symbol)
		{
			return &mTiles[i];
		}
	}

	return nullptr;
}
