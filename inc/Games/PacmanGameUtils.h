#pragma once

#include "Utils/Vec2D.h"
#include <vector>

static const int PACMAN_MOVEMENT_SPEED = 50; // per second

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