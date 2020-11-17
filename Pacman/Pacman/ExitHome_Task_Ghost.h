#ifndef _EXIT_HOME_STATE_TASK_GHOST_H_
#define _EXIT_HOME_STATE_TASK_GHOST_H_

#include "BaseState_Task_Ghost.h"

class ExitHomeState_Ghost final : public BaseState_Ghost
{
public:
	ExitHomeState_Ghost();
	~ExitHomeState_Ghost() override;

private:
	void OnEnter() override;
	void OnExit() override;
	void OnUpdate() override;
	void CheckTransitions() override;
};

#endif
