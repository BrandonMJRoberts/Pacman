#ifndef _RETURN_HOME_STATE_TASK_GHOST_H_
#define _RETURN_HOME_STATE_TASK_GHOST_H_

#include "BaseState_Task_Ghost.h"

class Ghost;

class ReturnHomeState_Ghost final : public BaseState_Ghost
{
public:
	ReturnHomeState_Ghost();
	~ReturnHomeState_Ghost() override;

	void OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection) override;
	void CheckTransitions(Ghost* ghost) override;

private:
	void OnEnter() override;
	void OnExit() override;
};

#endif
