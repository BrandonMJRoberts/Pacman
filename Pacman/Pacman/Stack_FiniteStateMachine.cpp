#include "Stack_FiniteStateMachine.h"

#include "ChaseState_Task_Ghost.h"
#include "FleeState_Task_Ghost.h"
#include "ExitHome_Task_Ghost.h"
#include "ReturnHome_Task_Ghost.h"

// --------------------------------------------------------------------------------//
// -----------------------------  GHOST  ------------------------------------------//
// --------------------------------------------------------------------------------//

Stack_FiniteStateMachine_Ghost::Stack_FiniteStateMachine_Ghost(GHOST_TYPE ghostColour, bool enabled)
{
	mStateMachineIsEnabled = enabled;
	mGhostColour           = ghostColour;

	if(mStateMachineIsEnabled)
		PushToStack(GHOST_STATE_TYPE::CHASE);
}

// ------------------------------------------------------------------------------- //

Stack_FiniteStateMachine_Ghost::~Stack_FiniteStateMachine_Ghost()
{
	while (mCurrentStack.size() > 0)
		PopStack();

	mCurrentStack.clear();
}

// ------------------------------------------------------------------------------- //

void Stack_FiniteStateMachine_Ghost::PushToStack(GHOST_STATE_TYPE newState)
{
	switch (newState)
	{
	case GHOST_STATE_TYPE::CHASE:
		mCurrentStack.push_back(new ChaseState_Ghost(mGhostColour));
	break;

	case GHOST_STATE_TYPE::FLEE:
		mCurrentStack.push_back(new FleeState_Ghost(mGhostColour));
	break;

	case GHOST_STATE_TYPE::RETURN_HOME:
		mCurrentStack.push_back(new ReturnHomeState_Ghost());
	break;

	case GHOST_STATE_TYPE::EXIT_HOME:
		mCurrentStack.push_back(new ExitHomeState_Ghost());
	break;

	default:
	break;
	}
}

// --------------------------------------------------------------------------------//

void Stack_FiniteStateMachine_Ghost::PopStack()
{
	if (mCurrentStack.size() > 0)
	{
		delete mCurrentStack[mCurrentStack.size() - 1];
		mCurrentStack.pop_back();
	}
}

// --------------------------------------------------------------------------------//

BaseState_Ghost* Stack_FiniteStateMachine_Ghost::PeekStack()
{
	if (mCurrentStack.size() > 0)
		return mCurrentStack[mCurrentStack.size() - 1];
	else
		return nullptr;
}

// --------------------------------------------------------------------------------//
// -----------------------------  PACMAN  -----------------------------------------//
// --------------------------------------------------------------------------------//

#include "BaseState_Task_Pacman.h"
#include "ChaseGhostsState_Task_Pacman.h"
#include "CollectDotsState_Task_Pacman.h"
#include "DodgeGhostsState_Task_Pacman.h"

// --------------------------------------------------------------------------------//

Stack_FiniteStateMachine_Pacman::Stack_FiniteStateMachine_Pacman(bool enabled)
{
	if (enabled)
	{
		// Default to a collect dots state
		PushToStack(PACMAN_STATE_TYPES::COLLECT_DOTS);
	}
}

// --------------------------------------------------------------------------------//

Stack_FiniteStateMachine_Pacman::~Stack_FiniteStateMachine_Pacman()
{
	while (mCurrentStack.size() > 0)
		PopStack();

	mCurrentStack.clear();
}

// --------------------------------------------------------------------------------//

void Stack_FiniteStateMachine_Pacman::PushToStack(PACMAN_STATE_TYPES newState)
{
	switch (newState)
	{
	case PACMAN_STATE_TYPES::CHASE:
		mCurrentStack.push_back(new ChaseGhostsState_Pacman());
	break;

	case PACMAN_STATE_TYPES::FLEE:
		mCurrentStack.push_back(new DodgeGhostsState_Pacman());
	break;

	case PACMAN_STATE_TYPES::COLLECT_DOTS:
		mCurrentStack.push_back(new CollectDotsGhostsState_Pacman());
	break;

	default: break;
	}
}

// --------------------------------------------------------------------------------//

void Stack_FiniteStateMachine_Pacman::PopStack()
{
	if (mCurrentStack.size() > 0)
	{
		delete mCurrentStack[mCurrentStack.size() - 1];
		mCurrentStack.pop_back();
	}
}

// --------------------------------------------------------------------------------//

BaseState_Pacman* Stack_FiniteStateMachine_Pacman::PeekStack()
{
	if (mCurrentStack.size() > 0)
		return mCurrentStack[mCurrentStack.size() - 1];
	else
		return nullptr;
}

// --------------------------------------------------------------------------------//