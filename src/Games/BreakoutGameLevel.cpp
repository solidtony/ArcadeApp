#include "Games/BreakoutGameLevel.h"

#include "Games/Block.h"
#include "Games/Ball.h"

#include "Shapes/AARectangle.h"

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

void BreakoutGameLevel::Update(uint32_t dt, Ball& ball)
{
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

	const int BLOCK_WIDTH = 28;
	const int BLOCK_HEIGHT = 10;

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