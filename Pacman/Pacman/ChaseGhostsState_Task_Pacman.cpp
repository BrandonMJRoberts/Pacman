#include "ChaseGhostsState_Task_Pacman.h"

#include "PacmanCharacter.h"
#include "DotsHandler.h"
#include "Stack_FiniteStateMachine.h"

#include "GameManager.h"

// ------------------------------------------------------------ //

ChaseGhostsState_Pacman::ChaseGhostsState_Pacman() : BaseState_Pacman()
{
	
}

// ------------------------------------------------------------ //

ChaseGhostsState_Pacman::~ChaseGhostsState_Pacman()
{

}

// ------------------------------------------------------------ //

void ChaseGhostsState_Pacman::OnUpdate(S2D::Vector2 currentPosition, S2D::Vector2& targetPositionRef, std::vector<S2D::Vector2> ghostPositions, DotsHandler& dotManager)
{
	if (ghostPositions.size() > 0)
	{
		unsigned int bestIndex = 0;
		float smallestDistance = 1000000000;

		// If we are chasing ghosts then calculate which ghost is closest to us and go towards that one
		for (unsigned int i = 0; i < ghostPositions.size(); i++)
		{
			// Make sure we only consider edable ghosts
			if ((mValidGhostsToEat.size() > i && mValidGhostsToEat[i]) && (currentPosition - ghostPositions[i]).LengthSquared() < smallestDistance)
			{
				smallestDistance = (currentPosition - ghostPositions[i]).LengthSquared();
				bestIndex = i;
			}
		}

		// Set the closest ghost to be the target
		targetPositionRef = ghostPositions[bestIndex];

		return;
	}
}

// ------------------------------------------------------------ //

void ChaseGhostsState_Pacman::CheckTransitions(PacmanCharacter& pacman, std::vector<S2D::Vector2> ghostPositions, std::vector<bool> validGhostsToEat)
{
	mValidGhostsToEat = validGhostsToEat;

	// If the player is no-longer powered up then we shouldn't be chasing ghosts, or if there are no edable ghosts in the maze
	if (validGhostsToEat.size() == 0 || !GameManager::Instance()->GetIsPlayerPoweredUp() || ghostPositions.size() == 0)
		pacman.GetStateMachine().PopStack();
}

// ------------------------------------------------------------ //

void ChaseGhostsState_Pacman::OnEnter()
{

}

// ------------------------------------------------------------ //

void ChaseGhostsState_Pacman::OnExit()
{

}

// ------------------------------------------------------------ //