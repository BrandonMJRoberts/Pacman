#ifndef _BASE_STATE_TASK_GHOST_H_
#define _BASE_STATE_TASK_GHOST_H_

#include "S2D/S2D.h"
#include "Commons.h"

class Ghost;

class BaseState_Ghost abstract
{
public:
	BaseState_Ghost();
	virtual ~BaseState_Ghost();

	virtual void OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection);
	virtual void CheckTransitions(Ghost* ghost);

protected:
	virtual void OnEnter();
	virtual void OnExit();

};

#endif