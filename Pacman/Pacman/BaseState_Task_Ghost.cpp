#include "BaseState_Task_Ghost.h"
#include "Ghost.h"

// ---------------------------------------------------- //

BaseState_Ghost::BaseState_Ghost()
{
	OnEnter();
}

// ---------------------------------------------------- //

BaseState_Ghost::~BaseState_Ghost()
{

}

// ---------------------------------------------------- //

void BaseState_Ghost::OnUpdate()
{

}

// ---------------------------------------------------- //

void BaseState_Ghost::OnEnter()
{

}

// ---------------------------------------------------- //

void BaseState_Ghost::OnExit()
{

}

// ---------------------------------------------------- //

void BaseState_Ghost::CheckTransitions(Ghost* ghost)
{
	if (ghost)
	{

	}
	else
		return;
}

// ---------------------------------------------------- //