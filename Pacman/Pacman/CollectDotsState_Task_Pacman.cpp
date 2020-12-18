#include "CollectDotsState_Task_Pacman.h"

#include "DotsHandler.h"
#include "PacmanCharacter.h"
#include "Stack_FiniteStateMachine.h"
#include "Constants.h"

// ---------------------------------------------------- //

CollectDotsGhostsState_Pacman::CollectDotsGhostsState_Pacman()
{
	OnEnter();
}

// ---------------------------------------------------- //

CollectDotsGhostsState_Pacman::~CollectDotsGhostsState_Pacman()
{

}

// ---------------------------------------------------- //

void CollectDotsGhostsState_Pacman::OnUpdate(S2D::Vector2 currentPosition, S2D::Vector2& targetPositionRef, std::vector<S2D::Vector2> ghostPositions, DotsHandler& dotManager)
{
	// We are not in a panic state, so lets just collect some dots
	targetPositionRef = dotManager.GetRandomDotPosition();

	// Now make sure that the position is in the centre of a segment, as if it is on the edge then pacman can miss it
	if ((int)targetPositionRef.X - targetPositionRef.X == 0.0f)
	{
		targetPositionRef.X = (int)targetPositionRef.X + 0.5f;
	}

	if ((int)targetPositionRef.Y - targetPositionRef.Y == 0.0f)
	{
		targetPositionRef.Y = (int)targetPositionRef.Y + 0.5f;
	}

}

// ---------------------------------------------------- //

void CollectDotsGhostsState_Pacman::CheckTransitions(PacmanCharacter& pacman, std::vector<S2D::Vector2> ghostPositions, std::vector<bool> validGhostsToEat)
{
	// If the ghosts are within a range then flee
	for (unsigned int i = 0; i < ghostPositions.size(); i++)
	{
		if ((pacman.GetCentrePosition() - ghostPositions[i]).Length() < DISTANCE_FROM_GHOST_BEFORE_FLEE)
		{
			pacman.GetStateMachine().PushToStack(PACMAN_STATE_TYPES::FLEE);
			return;
		}
	}
}

// ---------------------------------------------------- //

void CollectDotsGhostsState_Pacman::OnEnter()
{

}

// ---------------------------------------------------- //

void CollectDotsGhostsState_Pacman::OnExit()
{

}

// ---------------------------------------------------- //