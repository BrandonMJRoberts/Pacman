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
	mType                   = GHOST_STATE_TYPE::CHASE;
	mPreviousPoweredUpState = false;
}

// ---------------------------------------------------------------- //

void ChaseState_Ghost::OnExit()
{

}

// ---------------------------------------------------------------- //

void ChaseState_Ghost::OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection)
{
	S2D::Vector2 positionalOffset = S2D::Vector2();
	float        projectDistance  = 1.5f;

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
		// In actual pacman the orange ghost moves randomly until it goes within a range of pacman, but for simplicity ive left it to just chase pacman directly

		targetPositionRef = S2D::Vector2((float)((int)pacmanPos.X), (float)((int)pacmanPos.Y));
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
		bool currentPoweredUpState = GameManager::Instance()->GetIsPlayerPoweredUp();

		// From here we can transition into FLEE, but only if this is a fresh power-up collected
		if (currentPoweredUpState && currentPoweredUpState != mPreviousPoweredUpState)
		{
			ghost->GetStateMachine()->PushToStack(GHOST_STATE_TYPE::FLEE);	
			ghost->SetTargetPosition(ghost->GetCentrePosition());
		}

		// Make sure to save the new state
		mPreviousPoweredUpState = currentPoweredUpState;
	}
	else
		return;
}

// ---------------------------------------------------- //