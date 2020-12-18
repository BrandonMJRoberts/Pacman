#ifndef _CHASE_GHOSTS_STATE_TASK_PACMAN_H_
#define _CHASE_GHOSTS_STATE_TASK_PACMAN_H_

#include "BaseState_Task_Pacman.h"

class PacmanCharacter;
class DotsHandler;

class ChaseGhostsState_Pacman final : public BaseState_Pacman
{
public:
	ChaseGhostsState_Pacman();
	~ChaseGhostsState_Pacman() override;

	void OnUpdate(S2D::Vector2 currentPosition, S2D::Vector2& targetPositionRef, std::vector<S2D::Vector2> ghostPositions, DotsHandler& dotManager) override;
	void CheckTransitions(PacmanCharacter& pacman, std::vector<S2D::Vector2> ghostPositions, std::vector<bool> validGhostsToEat)                                                                    override;

protected:
	void OnEnter()             override;
	void OnExit()              override;

	std::vector<bool> mValidGhostsToEat;
};

#endif