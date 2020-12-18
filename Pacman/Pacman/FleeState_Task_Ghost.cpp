#include "FleeState_Task_Ghost.h"

#include "GameManager.h"
#include "Ghost.h"

// ---------------------------------------------------------------- //

FleeState_Ghost::FleeState_Ghost(GHOST_TYPE colourOfGhost) : BaseState_Ghost()
{
	mGhostColour = colourOfGhost;

	mPacmanPreviousPowerupState = false;

	//OnEnter();
}

// ---------------------------------------------------------------- //

FleeState_Ghost::~FleeState_Ghost()
{
	OnExit();
}

// ---------------------------------------------------------------- //

void FleeState_Ghost::OnEnter()
{
	mType = GHOST_STATE_TYPE::FLEE;
}

// ---------------------------------------------------------------- //

void FleeState_Ghost::OnExit()
{
}

// ---------------------------------------------------------------- //

void FleeState_Ghost::OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection)
{
	// Set the correct flee position
	switch (mGhostColour)
	{
	case GHOST_TYPE::RED:
		targetPositionRef = S2D::Vector2(3, 3); // Top left
	return;

	case GHOST_TYPE::PINK:
		targetPositionRef = S2D::Vector2(3, 28); // Bottom left
	return;

	case GHOST_TYPE::ORANGE:
		targetPositionRef = S2D::Vector2(24, 3); // Top right
	return;

	case GHOST_TYPE::BLUE:
		targetPositionRef = S2D::Vector2(24, 28); // Bottom right
	return;

	default:
	return;
	}
}

// ---------------------------------------------------------------- //

void FleeState_Ghost::CheckTransitions(Ghost* ghost)
{
	if (ghost)
	{
		bool playerIsPoweredUp = GameManager::Instance()->GetIsPlayerPoweredUp();

		// From this state we could go back to CHASE, or into GO TO HOME state
		if (!playerIsPoweredUp)
		{
			ghost->GetStateMachine()->PopStack();
			ghost->SetGhostIsFleeing(false);
			return;
		}

		// Pacman must have eaten this ghost then
		if (playerIsPoweredUp && !ghost->IsAlive())
		{
			// Tell the ghost to return to its home
			ghost->GetStateMachine()->PopStack(); // Remove the flee task 
			ghost->GetStateMachine()->PushToStack(GHOST_STATE_TYPE::RETURN_HOME); // Add the return home task
			ghost->SetGhostIsFleeing(false); // Set that the ghost is not fleeing anymore
			return;
		}

		// If pacman's powerup state has changed then update the ghost's flee state
		if (mPacmanPreviousPowerupState != playerIsPoweredUp)
		{
			ghost->SetGhostIsFleeing(playerIsPoweredUp);
			mPacmanPreviousPowerupState = playerIsPoweredUp;
		}
	}
	else
		return;
}

// ---------------------------------------------------- //