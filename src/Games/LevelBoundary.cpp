#include "Games/LevelBoundary.h"

#include "Games/Ball.h"
#include "Shapes/AARectangle.h"

LevelBoundary::LevelBoundary(const AARectangle& boundary)
{
	mIncluder.Init(boundary, true); // Reverse the normals
}

bool LevelBoundary::HasCollided(const Ball& ball, BoundaryEdge& edge)
{
	for (int i = 0; i < NUM_EDGES; ++i)
	{
		BoundaryEdge collisionEdge = mIncluder.GetEdge(static_cast<EdgeType>(i));
		if (HasCollidedWithEdge(ball, collisionEdge))
		{
			edge = collisionEdge;
			return true;
		}
	}
}

bool LevelBoundary::HasCollidedWithEdge(const Ball& ball, const BoundaryEdge& edge) const
{
	Vec2D pos = ball.GetPosition();// TODO remove later only testing
	return edge.edge.MinDistanceFrom(ball.GetPosition()) < ball.GetRadius();
}