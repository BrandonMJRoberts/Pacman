#ifndef _DODGE_GHOSTS_STATE_TASK_PACMAN_H_
#define _DODGE_GHOSTS_STATE_TASK_PACMAN_H_

#include "BaseState_Task_Pacman.h"

class PacmanCharacter;
class DotsHandler;

class DodgeGhostsState_Pacman final : public BaseState_Pacman
{
public:
	DodgeGhostsState_Pacman();
	~DodgeGhostsState_Pacman() override;

	void OnUpdate(S2D::Vector2 currentPosition, S2D::Vector2& targetPositionRef, std::vector<S2D::Vector2> ghostPositions, DotsHandler& dotManager) override;
	void CheckTransitions(PacmanCharacter& pacman, std::vector<S2D::Vector2> ghostPosition, std::vector<bool> validGhostsToEat)                                                                    override;

protected:
	void OnEnter()             override;
	void OnExit()              override;
};

#endif