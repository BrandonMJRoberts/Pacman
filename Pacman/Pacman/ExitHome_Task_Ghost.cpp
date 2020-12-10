#include "ExitHome_Task_Ghost.h"

#include "Ghost.h"

// ---------------------------------------------------------------- //

ExitHomeState_Ghost::ExitHomeState_Ghost() : BaseState_Ghost()
{
	//OnEnter();
}

// ---------------------------------------------------------------- //

ExitHomeState_Ghost::~ExitHomeState_Ghost()
{
	OnExit();
}

// ---------------------------------------------------------------- //

void ExitHomeState_Ghost::OnEnter()
{

}

// ---------------------------------------------------------------- //

void ExitHomeState_Ghost::OnExit()
{
}

// ---------------------------------------------------------------- //

void ExitHomeState_Ghost::OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection)
{
	
}

// ---------------------------------------------------------------- //

void ExitHomeState_Ghost::CheckTransitions(Ghost* ghost)
{
	ghost->SetTargetPosition(ghost->GetCentrePosition());

	if (ghost)
	{
		if (!ghost->GetIsHome())
		{
			// Then remove this task from the list
			ghost->GetStateMachine()->PopStack();
			ghost->SetIsAlive(true);
			return;
		}
	}
	else
		return;
}

// ---------------------------------------------------- //