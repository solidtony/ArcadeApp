#include "PacmanLevel.h"

#include "Utils/FileCommandLoader.h"
#include "Graphics/Screen.h"

bool PacmanLevel::Init(const std::string& levelPath)
{
	return LoadLevel(levelPath);
}

void PacmanLevel::Update(uint32_t dt)
{

}

void PacmanLevel::Draw(Screen screen)
{
	// Debug for walls
	for (const auto& wall : mWalls)
	{
		screen.Draw(wall.GetAARectangle(), Color::Blue());
	}
}

bool PacmanLevel::LoadLevel(const std::string& levelPath)
{
	FileCommandLoader fileLoader;

	Command tileWidthCommand;
	tileWidthCommand.command = "";

	return false;
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