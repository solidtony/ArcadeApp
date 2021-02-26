#include "Games/PacmanGameUtils.h"

Vec2D GetMovementVector(PacmanMovement direction)
{
	switch (direction)
	{
	case PacmanMovement::LEFT:
		return Vec2D(-1, 0);
		break;
	case PacmanMovement::DOWN:
		return Vec2D(0, 1);
		break;
	case PacmanMovement::RIGHT:
		return Vec2D(1, 0);
		break;
	case PacmanMovement::UP:
		return Vec2D(0, -1);
		break;
	}

	return Vec2D::Zero();
}

PacmanMovement GetOppositeDirection(PacmanMovement direction)
{
	switch (direction)
	{
	case PacmanMovement::LEFT:
		return PacmanMovement::RIGHT;
		break;
	case PacmanMovement::DOWN:
		return PacmanMovement::UP;
		break;
	case PacmanMovement::RIGHT:
		return PacmanMovement::LEFT;
		break;
	case PacmanMovement::UP:
		return PacmanMovement::DOWN;
		break;
	}

	return PacmanMovement::NONE;
}

std::vector<PacmanMovement> GetPerpendicularMovements(PacmanMovement direction)
{
	std::vector<PacmanMovement> perpendicularDirections;

	if (direction == PacmanMovement::DOWN || direction == PacmanMovement::UP)
	{
		perpendicularDirections.push_back(PacmanMovement::LEFT);
		perpendicularDirections.push_back(PacmanMovement::RIGHT);
	}
	else if (direction == PacmanMovement::LEFT || direction == PacmanMovement::RIGHT)
	{
		perpendicularDirections.push_back(PacmanMovement::UP);
		perpendicularDirections.push_back(PacmanMovement::DOWN);
	}
	else
	{
		perpendicularDirections.push_back(PacmanMovement::UP);
		perpendicularDirections.push_back(PacmanMovement::LEFT);
		perpendicularDirections.push_back(PacmanMovement::DOWN);
		perpendicularDirections.push_back(PacmanMovement::RIGHT);
	}

	return perpendicularDirections;
}