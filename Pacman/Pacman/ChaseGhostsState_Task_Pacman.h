#ifndef _CHASE_GHOSTS_STATE_TASK_PACMAN_H_
#define _CHASE_GHOSTS_TASK_PACMAN_H_

#include "BaseState_Task_Pacman.h"

class ChaseGhostsState_Pacman final : public BaseState_Pacman
{
public:
	ChaseGhostsState_Pacman();
	~ChaseGhostsState_Pacman() override;

	void OnUpdate() override;
	void CheckTransitions() override;

protected:
	void OnEnter() override;
	void OnExit() override;
};

#endif