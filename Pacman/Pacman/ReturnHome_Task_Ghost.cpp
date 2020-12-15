#include "ReturnHome_Task_Ghost.h"

#include "Ghost.h"

// ---------------------------------------------------------------- //

ReturnHomeState_Ghost::ReturnHomeState_Ghost() : BaseState_Ghost(), mHomeEntrancePosRight(S2D::Vector2(14, 11)), mHomeEntrancePosLeft(S2D::Vector2(13, 11)), mHomeCentrePos(S2D::Vector2(14, 12))
{
	OnEnter();
}

// ---------------------------------------------------------------- //

ReturnHomeState_Ghost::~ReturnHomeState_Ghost()
{
	OnExit();
}

// ---------------------------------------------------------------- //

void ReturnHomeState_Ghost::OnEnter()
{
	mGoingToEntrance = true;
	mGoingIntoHome   = false;

	mType = GHOST_STATE_TYPE::RETURN_HOME;
}

// ---------------------------------------------------------------- //

void ReturnHomeState_Ghost::OnExit()
{
}

// ---------------------------------------------------------------- //

void ReturnHomeState_Ghost::OnUpdate(S2D::Vector2& targetPositionRef, S2D::Vector2 pacmanPos, FACING_DIRECTION pacmanFacingDirection)
{
	// Making the ghost go to the door to the centre
	if (mGoingToEntrance)
	{
		targetPositionRef = mHomeEntrancePosLeft;
	}
	else // Making the ghost actually go into their home
	{
		targetPositionRef = mHomeCentrePos;
	}
}

// ---------------------------------------------------------------- //

void ReturnHomeState_Ghost::CheckTransitions(Ghost* ghost)
{
	// First check if the ghost exists
	if (ghost)
	{
		// If the ghost is going to the entrance of the home
		if (mGoingToEntrance)
		{
			S2D::Vector2 centrePos = ghost->GetCentrePosition();

			// If the ghost is in the correct position then tell them enter the home 
			if ((int(centrePos.X) == mHomeEntrancePosLeft.X &&
				 int(centrePos.Y) == mHomeEntrancePosLeft.Y) || 
				(int(centrePos.X) == mHomeEntrancePosRight.X &&
			     int(centrePos.Y) == mHomeEntrancePosRight.Y))
			{
				// Set we are not going to the entrance anymore
				mGoingToEntrance = false;
				mGoingIntoHome   = true;
			}
		}

		// If not going to the entrance means that they are going to their home position
		if (mGoingIntoHome)
		{
			S2D::Vector2 centrePos = ghost->GetCentrePosition();

			// Check if they are home
			if (int(centrePos.X) == mHomeCentrePos.X &&
				int(centrePos.Y) == mHomeCentrePos.Y)
			{
				// Set they are home
				ghost->SetIsHome(true);

				// Remove this task and then tell the ghost to leave the home
				ghost->GetStateMachine()->PopStack();
				ghost->GetStateMachine()->PushToStack(GHOST_STATE_TYPE::EXIT_HOME);
				return;
			}
		}
	}
	else
		return;
}

// ---------------------------------------------------------------- //