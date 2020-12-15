#include "BaseState_Task_Ghost.h"
#include "Ghost.h"

// ---------------------------------------------------- //

BaseState_Ghost::BaseState_Ghost()
{
}

// ---------------------------------------------------- //

BaseState_Ghost::~BaseState_Ghost()
{

}

// ---------------------------------------------------- //

void BaseState_Ghost::OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection)
{
	// This function is for calculating the target position of the ghost's movement
	// How it gets there is calculated in the ghost itself
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