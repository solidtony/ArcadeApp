#pragma once

#include "Utils/Vec2D.h"
#include <vector>

enum class PacmanMovement
{
	NONE = 0,
	UP,
	LEFT,
	DOWN,
	RIGHT
};

Vec2D GetMovementVector(PacmanMovement direction);
PacmanMovement GetOppositeDirection(PacmanMovement direction);
std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction);