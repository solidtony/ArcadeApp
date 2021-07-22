#pragma once

#include "Utils/Vec2D.h"
#include <vector>

static const int PACMAN_MOVEMENT_SPEED = 50; // per second
static const int GHOST_MOVEMENT_SPEED = 45;
static const int GHOST_VULNERABLE_MOVEMENT_SPEED = 25;
static const int GHOST_BACK_TO_PEN_SPEED = 100;

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
std::vector<PacmanMovement> GetOtherDirections(PacmanMovement direction);