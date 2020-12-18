#ifndef _FLEE_STATE_TASK_GHOST_H_
#define _FLEE_STATE_TASK_GHOST_H_

#include "BaseState_Task_Ghost.h"

class Ghost;

class FleeState_Ghost final : public BaseState_Ghost
{
public:
	FleeState_Ghost(GHOST_TYPE colourOfGhost);
	~FleeState_Ghost() override;

	void OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection) override;
	void CheckTransitions(Ghost* ghost) override;

private:
	void OnEnter() override;
	void OnExit() override;

	GHOST_TYPE mGhostColour;
	bool       mPacmanPreviousPowerupState;
};

#endif
