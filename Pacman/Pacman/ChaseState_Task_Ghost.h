#ifndef _CHASE_STATE_TASK_GHOST_H_
#define _CHASE_STATE_TASK_GHOST_H_

#include "BaseState_Task_Ghost.h"
#include "Commons.h"

class Ghost;
class PacmanCharacter;

class ChaseState_Ghost final : public BaseState_Ghost
{
public:
	ChaseState_Ghost(GHOST_TYPE ghostColour);
	~ChaseState_Ghost() override;

	void OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection) override;
	void CheckTransitions(Ghost* ghost) override;

private:
	void OnEnter() override;
	void OnExit() override;

	PacmanCharacter* mPacman;
	GHOST_TYPE       mColourOfGhost;
};

#endif
