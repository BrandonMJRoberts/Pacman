#include "BaseState_Task_Pacman.h"

#include "PacmanCharacter.h"
#include "DotsHandler.h"
#include "Stack_FiniteStateMachine.h"

// ---------------------------------------------------- //

BaseState_Pacman::BaseState_Pacman()
{
	OnEnter();
}

// ---------------------------------------------------- //

BaseState_Pacman::~BaseState_Pacman()
{

}

// ---------------------------------------------------- //

void BaseState_Pacman::OnUpdate(S2D::Vector2 currentPosition, S2D::Vector2& targetPositionRef, std::vector<S2D::Vector2> ghostPositions, DotsHandler& dotManager)
{

}

// ---------------------------------------------------- //

void BaseState_Pacman::CheckTransitions(PacmanCharacter& pacman, std::vector<S2D::Vector2> ghostPositions, std::vector<bool> validGhostsToEat)
{

}

// ---------------------------------------------------- //

void BaseState_Pacman::OnEnter()
{

}

// ---------------------------------------------------- //

void BaseState_Pacman::OnExit()
{

}

// ---------------------------------------------------- //