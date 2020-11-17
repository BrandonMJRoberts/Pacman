#ifndef _RETURN_HOME_STATE_TASK_GHOST_H_
#define _RETURN_HOME_STATE_TASK_GHOST_H_

#include "BaseState_Task_Ghost.h"

class ReturnHomeState_Ghost final : public BaseState_Ghost
{
public:
	ReturnHomeState_Ghost();
	~ReturnHomeState_Ghost() override;

	void OnUpdate() override;
	void CheckTransitions() override;

private:
	void OnEnter() override;
	void OnExit() override;
};

#endif
