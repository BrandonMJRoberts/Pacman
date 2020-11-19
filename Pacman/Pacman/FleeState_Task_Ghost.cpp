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

void FleeState_Ghost::OnUpdate()
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
			return;
		}

		// Must still have time remaining in the powerup
		if (ghost->IsEaten())
		{
			ghost->GetStateMachine()->PushToStack(GHOST_STATE_TYPE::RETURN_HOME);
			return;
		}
	}
	else
		return;
}

// ---------------------------------------------------- //