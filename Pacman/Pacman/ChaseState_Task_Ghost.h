#ifndef _CHASE_STATE_TASK_GHOST_H_
#define _CHASE_STATE_TASK_GHOST_H_

#include "BaseState_Task_Ghost.h"

class ChaseState_Ghost final : public BaseState_Ghost
{
public:
	ChaseState_Ghost();
	~ChaseState_Ghost() override;

	void OnUpdate() override;
	void CheckTransitions() override;

private:
	void OnEnter() override;
	void OnExit() override;
};

#endif
