#pragma once

#include "Shapes/Lines2D.h"

#include "Utils/Vec2D.h"

/*
			^ normal
			|
	----------------- edge
*/

enum EdgeType
{
	BOTTOM_EDGE = 0,
	TOP_EDGE,
	LEFT_EDGE,
	RIGHT_EDGE,
	NUM_EDGES // Gives the number of edges as long as BOTTOM_EDGE = 0
};

static const Vec2D UP_DIR =		{ 0.,-1. };
static const Vec2D DOWN_DIR =	{ 0., 1. };
static const Vec2D LEFT_DIR =	{-1., 0. };
static const Vec2D RIGHT_DIR =	{ 1., 0. };

struct BoundaryEdge
{
	Vec2D normal;
	Line2D edge;
};