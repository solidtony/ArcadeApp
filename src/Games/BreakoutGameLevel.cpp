#include "Games/BreakoutGameLevel.h"

#include <string>

#include "App/App.h"

#include "Games/Block.h"
#include "Games/Ball.h"

#include "Shapes/AARectangle.h"

#include "Utils/FileCommandLoader.h"

BreakoutGameLevel::BreakoutGameLevel()
{

}

void BreakoutGameLevel::Init(const AARectangle& boundary)
{
	CreateDefaultLevel(boundary);
}

void BreakoutGameLevel::Load(const std::vector<Block>& blocks)
{
	mBlocks.clear();
	mBlocks = blocks;
}

void BreakoutGameLevel::Update(uint32_t dt, Ball& ball, bool& wasBlockHitOUT)
{
	wasBlockHitOUT = false;

	std::vector<Block> collidedBlocks;

	BoundaryEdge edgeToBounceOffOf;
	Block* noptrBlockToBounceOffOf = nullptr;

	float largestMag = -1.f;

	for (auto& block : mBlocks)
	{
		BoundaryEdge edge;

		if (!block.IsDestroyed() &&
			block.HasCollided(ball.GetBoundingRect(), edge))
		{
			collidedBlocks.push_back(block);

			float mag = block.GetCollisionOffset(ball.GetBoundingRect()).Mag();

			if (mag > largestMag)
			{
				edgeToBounceOffOf = edge;
				noptrBlockToBounceOffOf = &block;
			}
		}
	}

	if (noptrBlockToBounceOffOf != nullptr)
	{
		noptrBlockToBounceOffOf->Bounce(ball, edgeToBounceOffOf);
		noptrBlockToBounceOffOf->ReduceHP();
		wasBlockHitOUT = true;
	}

	for (const auto& block : collidedBlocks)
	{
		BoundaryEdge edge;
		if (block.HasCollided(ball.GetBoundingRect(), edge))
		{
			Vec2D point;
			ball.MakeFlushWithEdge(edge, point, true);
		}
	}
}

void BreakoutGameLevel::Draw(Screen& screen)
{
	for (auto& block : mBlocks)
	{
		if (!block.IsDestroyed())
		{
			block.Draw(screen);
		}
	}
}

void BreakoutGameLevel::CreateDefaultLevel(const AARectangle& boundary)
{
	mBlocks.clear();

	const int NUM_BLOCKS_ACROSS = ((int)boundary.GetWidth() / (BLOCK_WIDTH + 1));
	const int NUM_BLOCK_ROWS = 5;

	float startX = ((int)boundary.GetWidth() - (NUM_BLOCKS_ACROSS * (BLOCK_WIDTH + 1.f))) / 2.f;

	Color colors[NUM_BLOCK_ROWS];
	colors[0] = Color::Red();
	colors[1] = Color::Magenta();
	colors[2] = Color::Yellow();
	colors[3] = Color::Green();
	colors[4] = Color::Cyan();

	for (int row = 0; row < NUM_BLOCK_ROWS; ++row)
	{
		AARectangle blockRect = { Vec2D(startX, BLOCK_HEIGHT * (row + 1.f)), static_cast<unsigned int>(BLOCK_WIDTH),  static_cast<unsigned int>(BLOCK_HEIGHT) };

		for (int col = 0; col < NUM_BLOCKS_ACROSS; ++col)
		{
			Block b;
			b.Init(blockRect, 1, Color::Black(), colors[row]);
			mBlocks.push_back(b);
			blockRect.MoveBy(Vec2D(BLOCK_WIDTH, 0));
		}
	}
}

bool BreakoutGameLevel::IsLevelComplete() const
{
	for (auto block : mBlocks)
	{
		if (!block.IsDestroyed() && block.GetHP() != Block::UNBREAKABLE)
		{
			return false;
		}
	}

	return true;
}

struct LayoutBlock
{
	char symbol = '-';
	int hp = 0;
	Color color = Color::Black();
};

LayoutBlock FindLayoutBlockForSymbol(const std::vector<LayoutBlock>& blocks, char symbol)
{
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		if (blocks[i].symbol == symbol)
		{
			return blocks[i];
		}
	}

	return LayoutBlock();
}

std::vector<BreakoutGameLevel> BreakoutGameLevel::LoadLevelsFromFile(const std::string& filePath)
{
	std::vector<BreakoutGameLevel> levels;

	std::vector<LayoutBlock> layoutBlocks;

	std::vector<Block> levelBlocks;

	int width = 0;
	int height = 0;

	FileCommandLoader fileLoader;

	// Level command
	Command levelCommand;
	levelCommand.command = "level";
	levelCommand.parseFunc = [&](ParseFuncParams params)
	{
		if (levels.size() > 0)
		{
			levels.back().Load(levelBlocks);
		}

		layoutBlocks.clear();
		levelBlocks.clear();
		width = 0;
		height = 0;

		BreakoutGameLevel level;
		level.Init(AARectangle(Vec2D::Zero(), App::Singleton().Width(), App::Singleton().Height()));

		levels.push_back(level);
	};

	fileLoader.AddCommand(levelCommand);

	// Block command
	Command blockCommand;
	blockCommand.command = "block";
	blockCommand.parseFunc = [&](ParseFuncParams params)
	{
		LayoutBlock lb;

		layoutBlocks.push_back(lb);
	};

	fileLoader.AddCommand(blockCommand);

	// Symbol command
	Command symbolCommand;
	symbolCommand.command = "symbol";
	symbolCommand.parseFunc = [&](ParseFuncParams params)
	{
		layoutBlocks.back().symbol = FileCommandLoader::ReadChar(params);
	};

	fileLoader.AddCommand(symbolCommand);

	// Fill color command
	Command fillColorCommand;
	fillColorCommand.command = "fillcolor";
	fillColorCommand.parseFunc = [&](ParseFuncParams params)
	{
		layoutBlocks.back().color = FileCommandLoader::ReadColor(params);
	};

	fileLoader.AddCommand(fillColorCommand);

	// HP command
	Command hpCommand;
	hpCommand.command = "hp";
	hpCommand.parseFunc = [&](ParseFuncParams params)
	{
		layoutBlocks.back().hp = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(hpCommand);

	// Width command
	Command widthCommand;
	widthCommand.command = "width";
	widthCommand.parseFunc = [&](ParseFuncParams params)
	{
		width = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(widthCommand);

	// Height command
	Command heightCommand;
	heightCommand.command = "height";
	heightCommand.parseFunc = [&](ParseFuncParams params)
	{
		height = FileCommandLoader::ReadInt(params);
	};

	fileLoader.AddCommand(heightCommand);

	// Layout command
	Command layoutCommand;
	layoutCommand.commandType = COMMAND_MULTI_LINE;
	layoutCommand.command = "layout";
	layoutCommand.parseFunc = [&](ParseFuncParams params)
	{
		int blockWidth = BLOCK_WIDTH;
		int screenWidth = App::Singleton().Width();

		float startingX = 0;
		AARectangle blockRect(Vec2D(startingX, (params.lineNum + 1)*BLOCK_HEIGHT), BLOCK_WIDTH, BLOCK_HEIGHT);

		for (int col = 0; col < params.line.length(); ++col)
		{
			if (params.line[col] != '-')
			{
				LayoutBlock layoutBlock = FindLayoutBlockForSymbol(layoutBlocks, params.line[col]);

				Block block;
				block.Init(blockRect, layoutBlock.hp, Color::Black(), layoutBlock.color);
				levelBlocks.push_back(block);
			}

			blockRect.MoveBy(Vec2D(BLOCK_WIDTH, 0));
		}
	};

	fileLoader.AddCommand(layoutCommand);

	// Load file
	fileLoader.LoadFile(filePath);

	if (levels.size() > 0)
	{
		levels.back().Load(levelBlocks);
	}

	return levels;
}