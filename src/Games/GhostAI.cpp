#include "Games/GhostAI.h"

#include "Graphics/Screen.h"
#include "Games/PacmanLevel.h"
#include "Games/Pacman.h"
#include "Graphics/Color.h"
#include "Shapes/Circle.h"
#include <cassert>
#include <algorithm>

namespace
{
	const uint32_t PEN_WAIT_DURATION = 5000;
	const uint32_t SCATTER_MODE_DURATION = 10000;
}

GhostAI::GhostAI() : mnoptrGhost(nullptr)
{
	
}

void GhostAI::Init(Ghost& ghost, uint32_t lookAheadDistance, const Vec2D& scatterTarget, GhostName name)
{
	mScatterTarget = scatterTarget;
	mLookAheadDistance = lookAheadDistance;
	mnoptrGhost = &ghost;
	mName = name;
	std::random_device r;
	mAIRandomGenerator.seed(r());
	mTimer = 0;
	SetState(GhostAIState::SCATTER);
	mLastState = GhostAIState::SCATTER;
}

PacmanMovement GhostAI::Update(uint32_t dt, const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost>& ghosts)
{
	if (mnoptrGhost != nullptr)
	{
		if (mState == GhostAIState::SCATTER)
		{
			mTimer += dt;
			if (mTimer >= SCATTER_MODE_DURATION)
			{
				SetState(GhostAIState::CHASE);
			}
		}

		PacmanMovement currentDirection = mnoptrGhost->GetMovementDirection();

		std::vector<PacmanMovement> tempDirections;
		std::vector<PacmanMovement> possibleDirections;

		possibleDirections = GetPerpendicularMovements(currentDirection);

		if (currentDirection != PacmanMovement::NONE)
		{
			possibleDirections.push_back(currentDirection);
		}

		for (const auto& pDirection : possibleDirections)
		{
			tempDirections.push_back(pDirection);
		}

		possibleDirections.clear();

		for (const auto& direction : tempDirections)
		{
			if (!level.WillCollide(mnoptrGhost->GetBoundingBox(), direction))
			{
				possibleDirections.push_back(direction);
			}
		}

		assert(possibleDirections.size() >= 1 && "Why can't we go anywhere?");

		std::sort(possibleDirections.begin(), possibleDirections.end(), [](const PacmanMovement& direction1, const PacmanMovement& direction2) {
			return direction1 < direction2;
		});

		if (mnoptrGhost->IsVulnerable() && (mState == GhostAIState::SCATTER || mState == GhostAIState::CHASE))
		{
			std::uniform_int_distribution<size_t> distribution(0, possibleDirections.size());
			return possibleDirections[static_cast<int>(distribution(mAIRandomGenerator))];
		}

		if (mState == GhostAIState::CHASE)
		{
			ChangeTarget(GetChaseTarget(dt, pacman, level, ghosts));
		}

		PacmanMovement directionToGoIn = PacmanMovement::NONE;

		uint32_t lowestDistance = UINT32_MAX;
		
		for (const auto& direction : possibleDirections)
		{
			Vec2D movementVec = GetMovementVector(direction) * mLookAheadDistance;
			AARectangle bbox = mnoptrGhost->GetBoundingBox();

			bbox.MoveBy(movementVec);

			uint32_t distanceToTarget = bbox.GetCenterPoint().Distance(mTarget);

			if (distanceToTarget < lowestDistance)
			{
				directionToGoIn = direction;
				lowestDistance = distanceToTarget;
			}
		}

		assert(directionToGoIn != PacmanMovement::NONE);

		return directionToGoIn;
	}

	return PacmanMovement::NONE;
}

void GhostAI::Draw(Screen& screen)
{
	if (mnoptrGhost != nullptr)
	{
		Circle targetCircle = Circle(mTarget, 4); // TODO debug code

		screen.Draw(targetCircle, mnoptrGhost->GetSpriteColor(), true, mnoptrGhost->GetSpriteColor());

		AARectangle bbox = mnoptrGhost->GetBoundingBox();

		bbox.MoveBy(GetMovementVector(mnoptrGhost->GetMovementDirection()) * mnoptrGhost->GetBoundingBox().GetWidth());
		
		Color c = Color(mnoptrGhost->GetSpriteColor().GetRed(), mnoptrGhost->GetSpriteColor().GetGreen(), mnoptrGhost->GetSpriteColor().GetBlue(), 200);

		screen.Draw(bbox, mnoptrGhost->GetSpriteColor(), true, c);
	}
}

void GhostAI::ChangeTarget(const Vec2D& target)
{
	mTarget = target;
}

Vec2D GhostAI::GetChaseTarget(uint32_t dt, const Pacman& pacman, const PacmanLevel& level, const std::vector<Ghost>& ghosts)
{
	Vec2D target;

	switch (mName)
	{
	case GhostName::BLINKY:
		target = pacman.GetBoundingBox().GetCenterPoint();
		break;
	case GhostName::PINKY:
		target = pacman.GetBoundingBox().GetCenterPoint() + 2 * GetMovementVector(pacman.GetMovementDirection()) * pacman.GetBoundingBox().GetWidth();
		break;
	case GhostName::INKY:
	{
		Vec2D pacmanOffsetPoint = pacman.GetBoundingBox().GetCenterPoint() + (GetMovementVector(pacman.GetMovementDirection()) * pacman.GetBoundingBox().GetWidth());
		target = (pacmanOffsetPoint - ghosts[GhostName::BLINKY].GetBoundingBox().GetCenterPoint() * 2 + ghosts[GhostName::BLINKY].GetBoundingBox().GetCenterPoint());
	}
		break;
	case GhostName::CLYDE:
	{
		auto distanceToPacman = mnoptrGhost->GetBoundingBox().GetCenterPoint().Distance(pacman.GetBoundingBox().GetCenterPoint());
		if (distanceToPacman > pacman.GetBoundingBox().GetWidth() * 4)
		{
			target = pacman.GetBoundingBox().GetCenterPoint();
		}
		else
		{
			target = mScatterTarget;
		}
	}
		break;
	}

	return target;
}

void GhostAI::SetState(GhostAIState state)
{
	if (mState == GhostAIState::SCATTER || mState == GhostAIState::CHASE)
	{
		mLastState = mState;
	}

	mState = state;

	switch (state)
	{
	case GhostAIState::IN_PEN:
		break;
	case GhostAIState::GO_TO_PEN:
		break;
	case GhostAIState::EXIT_PEN:
		break;
	case GhostAIState::SCATTER:
		mTimer = 0;
		ChangeTarget(mScatterTarget);
		break;
	default:
		break;
	}
}