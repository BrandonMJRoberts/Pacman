#include "ReturnHome_Task_Ghost.h"

#include "Ghost.h"

// ---------------------------------------------------------------- //

ReturnHomeState_Ghost::ReturnHomeState_Ghost() : BaseState_Ghost()
{
	//OnEnter();
}

// ---------------------------------------------------------------- //

ReturnHomeState_Ghost::~ReturnHomeState_Ghost()
{
	OnExit();
}

// ---------------------------------------------------------------- //

void ReturnHomeState_Ghost::OnEnter()
{

}

// ---------------------------------------------------------------- //

void ReturnHomeState_Ghost::OnExit()
{
}

// ---------------------------------------------------------------- //

void ReturnHomeState_Ghost::OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection)
{

}

// ---------------------------------------------------------------- //

void ReturnHomeState_Ghost::CheckTransitions(Ghost* ghost)
{
	if (ghost)
	{
		// We can go to the exit home state from here
		if (ghost->GetIsHome())
		{
			// Remove this task and then tell the ghost to leave the home
			ghost->GetStateMachine()->PopStack();
			ghost->GetStateMachine()->PushToStack(GHOST_STATE_TYPE::EXIT_HOME);
			return;
		}
	}
	else
		return;
}

// ---------------------------------------------------------------- //