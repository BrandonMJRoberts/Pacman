#include "Stack_FiniteStateMachine.h"

#include "ChaseState_Task_Ghost.h"

// --------------------------------------------------------------------------------//
// -----------------------------  GHOST  ------------------------------------------//
// --------------------------------------------------------------------------------//

Stack_FiniteStateMachine_Ghost::Stack_FiniteStateMachine_Ghost(bool enabled)
{
	mStateMachineIsEnabled = enabled;

	if(mStateMachineIsEnabled)
		PushToStack((BaseState_Ghost*) (new ChaseState_Ghost()));
}

// ------------------------------------------------------------------------------- //

Stack_FiniteStateMachine_Ghost::~Stack_FiniteStateMachine_Ghost()
{
	while (mCurrentStack.size() > 0)
		PopStack();

	mCurrentStack.clear();
}

// ------------------------------------------------------------------------------- //

void Stack_FiniteStateMachine_Ghost::PushToStack(BaseState_Ghost* newState)
{
	mCurrentStack.push_back(newState);
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

Stack_FiniteStateMachine_Pacman::Stack_FiniteStateMachine_Pacman()
{

}

// --------------------------------------------------------------------------------//

Stack_FiniteStateMachine_Pacman::~Stack_FiniteStateMachine_Pacman()
{
	while (mCurrentStack.size() > 0)
		PopStack();

	mCurrentStack.clear();
}

// --------------------------------------------------------------------------------//

void Stack_FiniteStateMachine_Pacman::PushToStack(BaseState_Pacman* newState)
{

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