#include "ChaseState_Task_Ghost.h"

#include "Ghost.h"
#include "GameManager.h"

// ---------------------------------------------------------------- //

ChaseState_Ghost::ChaseState_Ghost() : BaseState_Ghost()
{
	//OnEnter();
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

void ChaseState_Ghost::OnUpdate()
{
	
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