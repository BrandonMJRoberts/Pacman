#include "DodgeGhostsState_Task_Pacman.h"

#include "DotsHandler.h"
#include "PacmanCharacter.h"
#include "Stack_FiniteStateMachine.h"

#include "GameManager.h"

// ---------------------------------------------------- //

DodgeGhostsState_Pacman::DodgeGhostsState_Pacman()
{
	OnEnter();
}

// ---------------------------------------------------- //

DodgeGhostsState_Pacman::~DodgeGhostsState_Pacman()
{

}

// ---------------------------------------------------- //

void DodgeGhostsState_Pacman::OnUpdate(S2D::Vector2 currentPosition, S2D::Vector2& targetPositionRef, std::vector<S2D::Vector2> ghostPositions, DotsHandler& dotManager)
{
	// We need to dodge the ghosts so lets first check if there are any power-ups remaining
	if (dotManager.GetLargeDotPositions().size() > 0)
	{
		// There are power-ups remaining so lets try and get one

		// Now we need to see if any of the dots are reasonable to go for - aka not blocked by any ghosts
		// How this is done is through checking if there is a ghost on the same side of us as the power-up
		std::vector<S2D::Vector2> dotPositions = dotManager.GetLargeDotPositions();

		// Defined here to prevent constant re-allocation of memory
		S2D::Vector2 powerupDifferential, ghostDifferential;

		for (unsigned int i = 0; i < dotPositions.size(); i++)
		{
			bool idealDot = false;

			powerupDifferential = dotPositions[i] - currentPosition;

			// Now loop through all of the ghost positions and check if they block of the path
			for (unsigned int j = 0; j < ghostPositions.size(); j++)
			{
				// Now get the differential from our position to the power-up
				ghostDifferential   = ghostPositions[j] - currentPosition;

				// Now lets see which direction want to go from here
				if ((int)powerupDifferential.X > 0)
				{
					if ((int)powerupDifferential.Y > 0)
					{
						if ((int)ghostDifferential.X > 0 && (int)(ghostDifferential.Y) > 0)
						{
							idealDot = false;
							break;
						}
					}
					else if ((int)powerupDifferential.Y < 0)
					{
						if ((int)ghostDifferential.X > 0 && (int)(ghostDifferential.Y) < 0)
						{
							idealDot = false;
							break;
						}
					}
				}
				else if ((int)powerupDifferential.X < 0)
				{
					if ((int)powerupDifferential.Y > 0)
					{
						if ((int)ghostDifferential.X < 0 && (int)(ghostDifferential.Y) > 0)
						{
							idealDot = false;
							break;
						}

					}
					else if ((int)powerupDifferential.Y < 0)
					{
						if ((int)ghostDifferential.X < 0 && (int)(ghostDifferential.Y) < 0)
						{
							idealDot = false;
							break;
						}
					}
				}
			}

			if (idealDot)
			{
				targetPositionRef = dotPositions[i];
				return;
			}
		}
	}

	// There are no power-ups remaining so we just need to dodge the ghosts as well as we can
	unsigned int directionBias[4] = { 0,0,0,0 }; // In the same order of FACING_DIRECTION, up, down, left, right

	S2D::Vector2 ghostDifferential;

	// Loop through all ghost positions
	for (unsigned int i = 0; i < ghostPositions.size(); i++)
	{
		// Get the movement differential
		ghostDifferential = ghostPositions[i] - currentPosition;

		if (ghostDifferential.Y > 0)
		{
			directionBias[0]++;
		}
		
		if (ghostDifferential.Y < 0)
		{
			directionBias[1]++;
		}

		if (ghostDifferential.X < 0)
		{
			directionBias[2]++;
		}

		if (ghostDifferential.X > 0)
		{
			directionBias[3]++;
		}
	}

	// Now move in the direction that has the least ghost traces in it
	unsigned int currentLowestScore = directionBias[0], currentLowestIndex = 0;
	for (unsigned int i = 1; i < 4; i++)
	{
		if (directionBias[i] < currentLowestScore)
			currentLowestIndex = i;
	}

	// Now set that pacman should move in a certain direction
	if(currentLowestIndex == 0)
		targetPositionRef = currentPosition + S2D::Vector2(0.0f, -1.0f);
	else if(currentLowestIndex == 1)
		targetPositionRef = currentPosition + S2D::Vector2(0.0f, 1.0f);
	else if(currentLowestIndex == 2)
		targetPositionRef = currentPosition + S2D::Vector2(-1.0f, 0.0f);
	else
		targetPositionRef = currentPosition + S2D::Vector2(1.0f, 0.0f);

	return;
}

// ---------------------------------------------------- //

void DodgeGhostsState_Pacman::CheckTransitions(PacmanCharacter& pacman, std::vector<S2D::Vector2> ghostPositions)
{
	if (GameManager::Instance()->GetIsPlayerPoweredUp())
	{
		// Remove this state and add chase to the stack
		pacman.GetStateMachine().PopStack();
		pacman.GetStateMachine().PushToStack(PACMAN_STATE_TYPES::CHASE);
		return;
	}

	for (unsigned int i = 0; i < ghostPositions.size(); i++)
	{
		// The + 2 is so we dont keep swapping in and out of flee state
		if ((ghostPositions[i] - pacman.GetCentrePosition()).Length() < (DISTANCE_FROM_GHOST_BEFORE_FLEE + 2))
		{
			// It is not safe to exit the flee state
			return;
		}
	}

	// If it is safe to leave the flee state then do so
	pacman.GetStateMachine().PopStack();
}

// ---------------------------------------------------- //

void DodgeGhostsState_Pacman::OnEnter()
{

}

// ---------------------------------------------------- //

void DodgeGhostsState_Pacman::OnExit()
{

}

// ---------------------------------------------------- //