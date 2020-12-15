#include "ExitHome_Task_Ghost.h"

#include "AudioManager.h"

#include "Ghost.h"

// ---------------------------------------------------------------- //

ExitHomeState_Ghost::ExitHomeState_Ghost() : BaseState_Ghost(), mHomeEntrancePosRight(S2D::Vector2(14, 11)), mHomeEntrancePosLeft(S2D::Vector2(13, 11))
{
	OnEnter();
}

// ---------------------------------------------------------------- //

ExitHomeState_Ghost::~ExitHomeState_Ghost()
{
	OnExit();
}

// ---------------------------------------------------------------- //

void ExitHomeState_Ghost::OnEnter()
{
	mType = GHOST_STATE_TYPE::EXIT_HOME;
}

// ---------------------------------------------------------------- //

void ExitHomeState_Ghost::OnExit()
{
}

// ---------------------------------------------------------------- //

void ExitHomeState_Ghost::OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection)
{

}

// ---------------------------------------------------------------- //

void ExitHomeState_Ghost::CheckTransitions(Ghost* ghost)
{
	// If the ghost exists
	if (ghost)
	{
		// If the ghost is home and eaten then reset them to not being eaten
		if (ghost->GetIsHome() && ghost->GetIfGhostIsEaten())
		{
			ghost->SetGhostIsEaten(false);            // Set the ghost to no longer be eaten
			ghost->SetIsAlive(true);                  // Make sure the ghost is alive
			ghost->SetGhostIsFleeing(false);          // Make sure the ghost is no longer fleeing pacman

			// Pause the audio that plays when the ghosts are going home
			AudioManager::GetInstance()->PauseGhostGoingToHomeSFX();

			return;
		}
	
		// If the ghost can leave then they should exit as soon as they can
		if (ghost->GetCanLeaveHome())
		{
			// If not exited then set the target position to be the entrance of the home
			ghost->SetTargetPosition(mHomeEntrancePosLeft);

			// If the ghost has left the home then we need to close the door behind the ghost
			S2D::Vector2 position = ghost->GetCentrePosition();

			if ((int(position.X) == mHomeEntrancePosLeft.X &&
				 int(position.Y) == mHomeEntrancePosLeft.Y) ||
				(int(position.X) == mHomeEntrancePosRight.X &&
				 int(position.Y) == mHomeEntrancePosRight.Y))
			{
				// Then remove this task from the list
				ghost->GetStateMachine()->PopStack();

				return;
			}
		}
	}
	else
		return;
}

// ---------------------------------------------------------------- //