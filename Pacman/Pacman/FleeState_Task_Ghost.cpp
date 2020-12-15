#include "FleeState_Task_Ghost.h"

#include "GameManager.h"
#include "Ghost.h"

// ---------------------------------------------------------------- //

FleeState_Ghost::FleeState_Ghost(GHOST_TYPE colourOfGhost) : BaseState_Ghost()
{
	mGhostColour = colourOfGhost;

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
		// From this state we could go back to CHASE, or into GO TO HOME state
		if (!GameManager::Instance()->GetIsPlayerPoweredUp())
		{
			ghost->GetStateMachine()->PopStack();
			ghost->SetGhostIsFleeing(false);
			return;
		}

		// Pacman must have eaten this ghost then
		if (GameManager::Instance()->GetIsPlayerPoweredUp() && !ghost->IsAlive())
		{
			// Tell the ghost to return to its home
			ghost->GetStateMachine()->PopStack(); // Remove the flee task 
			ghost->GetStateMachine()->PushToStack(GHOST_STATE_TYPE::RETURN_HOME); // Add the return home task
			ghost->SetGhostIsFleeing(false); // Set that the ghost is not fleeing anymore
			return;
		}

		ghost->SetGhostIsFleeing(true);
	}
	else
		return;
}

// ---------------------------------------------------- //