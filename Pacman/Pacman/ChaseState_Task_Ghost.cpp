#include "ChaseState_Task_Ghost.h"

#include "Ghost.h"
#include "GameManager.h"
#include "PacmanCharacter.h"

// ---------------------------------------------------------------- //

ChaseState_Ghost::ChaseState_Ghost(GHOST_TYPE ghostColour) : BaseState_Ghost()
{
	mColourOfGhost = ghostColour;
	OnEnter();
}

// ---------------------------------------------------------------- //

ChaseState_Ghost::~ChaseState_Ghost()
{
	OnExit();
}

// ---------------------------------------------------------------- //

void ChaseState_Ghost::OnEnter()
{

}

// ---------------------------------------------------------------- //

void ChaseState_Ghost::OnExit()
{

}

// ---------------------------------------------------------------- //

void ChaseState_Ghost::OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection)
{
	S2D::Vector2 positionalOffset = S2D::Vector2();

	switch (mColourOfGhost)
	{
	case GHOST_TYPE::RED:
		// To pos
		targetPositionRef = pacmanPos;
	return;

	case GHOST_TYPE::PINK:
		// In front
		switch (pacmanFacingDirection)
		{
		case FACING_DIRECTION::UP:
			positionalOffset.Y = -1;
		break;

		case FACING_DIRECTION::DOWN:
			positionalOffset.Y = 1;
		break;

		case FACING_DIRECTION::LEFT:
			positionalOffset.X = -1;
		break;

		case FACING_DIRECTION::RIGHT:
			positionalOffset.X = 1;
		break;

		default:
		break;
		}

		// If so then set to move there
		targetPositionRef = pacmanPos + positionalOffset;
	return;

	case GHOST_TYPE::ORANGE:
		// Random
		//S2D::Vector2 randomPos = S2D::Vector2(-1, -1);

		// While we havent picked a valid random position then pick another one
		//while (!CheckIsValidMove(randomPos))
		//{
		//	randomPos.X = rand()%;
		//	randomPos.Y = rand()%;
		//}

		//targetPositionRef = randomPos;
		//return;
	break;

	case GHOST_TYPE::BLUE:
		// In front
		switch (pacmanFacingDirection)
		{
		case FACING_DIRECTION::UP:
			positionalOffset.Y = 1;
		break;

		case FACING_DIRECTION::DOWN:
			positionalOffset.Y = -1;
		break;

		case FACING_DIRECTION::LEFT:
			positionalOffset.X = 1;
		break;

		case FACING_DIRECTION::RIGHT:
			positionalOffset.X = -1;
		break;

		default:
		break;
		}

		targetPositionRef = pacmanPos + positionalOffset;
	return;

	default:
	return;
	}
}

// ---------------------------------------------------------------- //

void ChaseState_Ghost::CheckTransitions(Ghost* ghost)
{
	// If the ghost passed in is valid
	if (ghost)
	{
		// From here we can transition into FLEE only
		if (GameManager::Instance()->GetIsPlayerPoweredUp())
			ghost->GetStateMachine()->PushToStack(GHOST_STATE_TYPE::FLEE);
	}
	else
		return;
}

// ---------------------------------------------------- //