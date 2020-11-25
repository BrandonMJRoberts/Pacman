#include "FleeState_Task_Ghost.h"

#include "GameManager.h"
#include "Ghost.h"

// ---------------------------------------------------------------- //

FleeState_Ghost::FleeState_Ghost() : BaseState_Ghost()
{
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

}

// ---------------------------------------------------------------- //

void FleeState_Ghost::OnExit()
{
}

// ---------------------------------------------------------------- //

void FleeState_Ghost::OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection)
{
	
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
		if (!ghost->IsAlive())
		{
			ghost->GetStateMachine()->PushToStack(GHOST_STATE_TYPE::RETURN_HOME);
			ghost->SetGhostIsFleeing(false);
			return;
		}

		ghost->SetGhostIsFleeing(true);
	}
	else
		return;
}

// ---------------------------------------------------- //