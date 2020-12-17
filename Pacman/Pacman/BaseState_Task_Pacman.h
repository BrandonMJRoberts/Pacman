#ifndef _BASE_STATE_TASK_PACMAN_H_
#define _BASE_STATE_TASK_PACMAN_H_

#include "S2D/S2D.h"

class PacmanCharacter;
class DotsHandler;

class BaseState_Pacman abstract
{
public:
	BaseState_Pacman();
	virtual     ~BaseState_Pacman();

	virtual void OnUpdate(S2D::Vector2 currentPosition, S2D::Vector2& targetPositionRef, std::vector<S2D::Vector2> ghostPositions, DotsHandler& dotManager);
	virtual void CheckTransitions(PacmanCharacter& pacman, std::vector<S2D::Vector2> ghostPositions);

protected:
	virtual void OnEnter();
	virtual void OnExit();
};

#endif