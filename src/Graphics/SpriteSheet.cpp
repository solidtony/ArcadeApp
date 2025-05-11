#include "Graphics/SpriteSheet.h"

#include <string>

#include "App/App.h"
#include "Utils/Utils.h"
#include "Utils/FileCommandLoader.h"

SpriteSheet::SpriteSheet()
{

}

bool SpriteSheet::Load(const std::string& name)
{
	bool loadedImage = mBMPImage.Load(App::Singleton().GetBasePath() + std::string("res/") + name + ".bmp");
	bool loadedSpriteSections = LoadSpriteSections(App::Singleton().GetBasePath() + std::string("res/") + name + ".txt");

	return loadedImage && loadedSpriteSections;
}

Sprite SpriteSheet::GetSprite(const std::string& spriteName) const
{
	size_t length = mSections.size();

	for (size_t i = 0; i < length; ++i)
	{
		if (StringCompare(mSections[i].key, spriteName))
		{
			return mSections[i].sprite;
		}
	}

	return Sprite();
}

std::vector<std::string> SpriteSheet::SpriteNames() const
{
	std::vector<std::string> spriteNames;

	for (const auto& section : mSections)
	{
		spriteNames.push_back(section.key);
	}

	return spriteNames;
}

bool SpriteSheet::LoadSpriteSections(const std::string& path)
{
	FileCommandLoader fileLoader;

	// Sprite command
	Command spriteCommand;
	spriteCommand.command = "sprite";

	spriteCommand.parseFunc = [&](ParseFuncParams params)
	{
		BMPImageSection section;
		mSections.push_back(section);
	};

	fileLoader.AddCommand(spriteCommand);

	// Key command
	Command keyCommand;
	keyCommand.command = "key";

	keyCommand.parseFunc = [&](ParseFuncParams params)
	{
		mSections.back().key = FileCommandLoader::ReadString(params);
	};

	fileLoader.AddCommand(keyCommand);

	// xPos command
	Command xPosCommand;
	xPosCommand.command = "xPos";
	xPosCommand.parseFunc = [&](ParseFuncParams params)
	{
		mSections.back().sprite.xPos = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(xPosCommand);

	// yPos command
	Command yPosCommand;
	yPosCommand.command = "yPos";
	yPosCommand.parseFunc = [&](ParseFuncParams params)
	{
		mSections.back().sprite.yPos = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(yPosCommand);

	// width command
	Command widthCommand;
	widthCommand.command = "width";
	widthCommand.parseFunc = [&](ParseFuncParams params)
	{
		mSections.back().sprite.width = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(widthCommand);

	// height command
	Command heightCommand;
	heightCommand.command = "height";

	heightCommand.parseFunc = [&](ParseFuncParams params)
	{
		mSections.back().sprite.height = FileCommandLoader::ReadInt(params);
	};
	fileLoader.AddCommand(heightCommand);

	return fileLoader.LoadFile(path);
}
