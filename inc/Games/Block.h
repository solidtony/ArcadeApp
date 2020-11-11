#pragma once

#include "Excluder.h"
#include "Graphics/Color.h"

class Ball;
class Screen;
struct BoundaryEdge;

class Block : public Excluder
{
public:
	static constexpr int UNBREAKABLE = -1;

	Block();

	void Init(const AARectangle& rect, int hp, const Color& outlineColor, const Color& fillColor);
	void Draw(Screen& screen);
	void Bounce(Ball& ball, const BoundaryEdge& edgeOUT);

	void ReduceHP();
	inline int GetHP() const { return mHp; }
	inline bool IsDestroyed() const { return mHp == 0; }
	inline const Color& GetOutlineColor() const { return mOutlineColor; }
	inline const Color& GetFillColor() const { return mFillColor; }

private:
	int mHp; // -1 = unbreakable

	Color mOutlineColor;
	Color mFillColor;
};