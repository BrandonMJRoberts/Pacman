#ifndef _STACK_FINITE_STATE_MACHINE_H_
#define _STACK_FINITE_STATE_MACHINE_H_

#include <vector>

#include "BaseState_Task_Ghost.h"
#include "BaseState_Task_Pacman.h"

#include "Commons.h"

// ------------------------------------------------------------------------------------------------------ //

// Ghost's state machine
class Stack_FiniteStateMachine_Ghost final
{
public:
	Stack_FiniteStateMachine_Ghost() = delete;
	Stack_FiniteStateMachine_Ghost(GHOST_TYPE ghostColour, bool enabled = true);
	~Stack_FiniteStateMachine_Ghost();

	BaseState_Ghost*        PeekStack();
	void                    PopStack();
	void                    PushToStack(GHOST_STATE_TYPE newState);

private:
	std::vector<BaseState_Ghost*> mCurrentStack;

	GHOST_TYPE                    mGhostColour;
	bool                          mStateMachineIsEnabled;
};

// ------------------------------------------------------------------------------------------------------ //

// Pacman's state machine
class Stack_FiniteStateMachine_Pacman final
{
public:
	Stack_FiniteStateMachine_Pacman(bool enabled);
	~Stack_FiniteStateMachine_Pacman();

	BaseState_Pacman*		 PeekStack();
	void                     PopStack();
	void                     PushToStack(PACMAN_STATE_TYPES newState);

private:
	std::vector<BaseState_Pacman*> mCurrentStack;
};

// ------------------------------------------------------------------------------------------------------ //

#endif
