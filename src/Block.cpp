#include "Games/Block.h"

#include "Games/Ball.h"
#include "Games/BoundaryEdge.h"

#include "Graphics/Screen.h"

Block::Block() : mHp(1), mOutlineColor(Color::White()), mFillColor(Color::White())
{

}

void Block::Init(const AARectangle& rect, int hp, const Color& outlineColor, const Color& fillColor)
{
	Excluder::Init(rect);
	mHp = hp;
	mOutlineColor = outlineColor;
	mFillColor = fillColor;
}

void Block::Draw(Screen& screen)
{
	screen.Draw(GetAARectangle(), mOutlineColor, true, mFillColor);
}

void Block::Bounce(Ball& ball, const BoundaryEdge& edgeOUT)
{
	ball.Bounce(edgeOUT);
}

void Block::ReduceHP()
{
	if (mHp > 0)
	{
		--mHp;
	}
}