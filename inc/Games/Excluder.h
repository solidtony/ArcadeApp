#pragma once

#include "Games/BoundaryEdge.h"

#include "Shapes/AARectangle.h"

class Excluder
{
public:
	virtual ~Excluder() {}
	void Init(const AARectangle& rect, bool reverseNormals = false);
	bool HasCollided(const AARectangle& rect, BoundaryEdge& edgeOUT) const;
	Vec2D GetCollisionOffset(const AARectangle& rect) const;
	inline const AARectangle& GetAARectangle() const { return mAARect; }
	void MoveBy(const Vec2D& delta);
	void MoveTo(const Vec2D& point);
	const BoundaryEdge& GetEdge(EdgeType edge) const;

private:
	void SetupEdges();
	AARectangle mAARect;
	BoundaryEdge mEdges[NUM_EDGES];
	bool mReverseNormals;
};