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
	float        projectDistance  = 0.2f;

	switch (mColourOfGhost)
	{
	case GHOST_TYPE::RED:
		// Go to pacman's exact position
		targetPositionRef = pacmanPos;
	return;

	case GHOST_TYPE::PINK:
		// Go in front of pacman
		switch (pacmanFacingDirection)
		{
		case FACING_DIRECTION::UP:
			positionalOffset.Y = -projectDistance;
		break;

		case FACING_DIRECTION::DOWN:
			positionalOffset.Y = projectDistance;
		break;

		case FACING_DIRECTION::LEFT:
			positionalOffset.X = -projectDistance;
		break;

		case FACING_DIRECTION::RIGHT:
			positionalOffset.X = projectDistance;
		break;

		default:
		break;
		}

		targetPositionRef = S2D::Vector2((int)pacmanPos.X + positionalOffset.X, (int)pacmanPos.Y + positionalOffset.Y);
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

		targetPositionRef = S2D::Vector2((int)pacmanPos.X + positionalOffset.X, (int)pacmanPos.Y + positionalOffset.Y);
	return;

	case GHOST_TYPE::BLUE:

		// In front
		switch (pacmanFacingDirection)
		{
		case FACING_DIRECTION::UP:
			positionalOffset.Y = projectDistance;
		break;

		case FACING_DIRECTION::DOWN:
			positionalOffset.Y = -projectDistance;
		break;

		case FACING_DIRECTION::LEFT:
			positionalOffset.X = projectDistance;
		break;

		case FACING_DIRECTION::RIGHT:
			positionalOffset.X = -projectDistance;
		break;

		default:
		break;
		}

		targetPositionRef = S2D::Vector2((int)pacmanPos.X + positionalOffset.X, (int)pacmanPos.Y + positionalOffset.Y);
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
		{
			ghost->GetStateMachine()->PushToStack(GHOST_STATE_TYPE::FLEE);	
			ghost->SetTargetPosition(ghost->GetCentrePosition());
		}
	}
	else
		return;
}

// ---------------------------------------------------- //