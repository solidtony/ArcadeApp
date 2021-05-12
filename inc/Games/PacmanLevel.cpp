#include "PacmanLevel.h"

#include "Utils/FileCommandLoader.h"
#include "Graphics/Screen.h"
#include "Games/Pacman.h"
#include "App/App.h"
#include "Shapes/Circle.h"
#include "Shapes/AARectangle.h"
#include <cassert>

bool PacmanLevel::Init(const std::string& levelPath, Pacman* noptrPacman)
{
	mnoptrPacman = noptrPacman;

	bool wasLevelLoaded = LoadLevel(levelPath);

	if (wasLevelLoaded)
	{
		ResetLevel();
	}

	return wasLevelLoaded;
}

void PacmanLevel::Update(uint32_t dt)
{
	assert(mnoptrPacman);

	for (const auto& wall : mWalls)
	{
		BoundaryEdge edge;

		if (wall.HasCollided(mnoptrPacman->GetBoundingBox(), edge))
		{
			Vec2D offset = wall.GetCollisionOffset(mnoptrPacman->GetBoundingBox());
			mnoptrPacman->MoveBy(offset);
			mnoptrPacman->Stop();
		}
	}

	for (Tile t : mTiles)
	{
		if (t.isTeleportTile)
		{
			AARectangle teleportTileAABB(t.position, t.width, static_cast<float>(mTileHeight));

			Tile* teleportToTile = GetTileForSymbol(t.teleportToSymbol);
			assert(teleportToTile);

			if (teleportToTile->isTeleportTile && teleportTileAABB.Intersects(mnoptrPacman->GetBoundingBox()))
			{
				mnoptrPacman->MoveTo(teleportToTile->position + teleportToTile->offset);
			}
		}
	}

	for (auto& pellet : mPellets)
	{
		if (!pellet.isEaten)
		{
			if (mnoptrPacman->GetEatingBoundingBox().Intersects(pellet.mBBox))
			{
				pellet.isEaten = true;

				mnoptrPacman->AteItem(pellet.score);

				if (pellet.isPowerPellet)
				{
					mnoptrPacman->ResetGhostEatenMultiplier();
					// TODO: Make ghosts vulnerable
				}
			}
		}
	}
}

void PacmanLevel::Draw(Screen& screen)
{
	// Debug for walls
	for (const auto& wall : mWalls)
	{
		screen.Draw(wall.GetAARectangle(), Color::Blue());
	}

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
}

bool PacmanLevel::WillCollide(const AARectangle& aBBox, PacmanMovement direction) const
{
	AARectangle bbox = aBBox;

	bbox.MoveBy(GetMovementVector(direction));

	for (const auto& wall : mWalls)
	{
		BoundaryEdge edge;

		if(wall.HasCollided(bbox, edge))
		{
			return true;
		}
	}

	return false;
}

void PacmanLevel::ResetLevel()
{
	ResetPellets();
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
				if (tile->collidable > 0)
				{
					Excluder wall;
					wall.Init(AARectangle(Vec2D(startingX, layoutOffset.GetY()), tile->width, static_cast<int>(mTileHeight)));
					mWalls.push_back(wall);
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