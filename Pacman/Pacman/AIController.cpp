#include "AIController.h"

#include "Stack_FiniteStateMachine.h"
#include "BaseState_Task_Pacman.h"

#include "Ghost.h"
#include "PacmanCharacter.h"

#include "Constants.h"
#include "GameManager.h"

// ---------------------------------------------------------------------- //

AIController::AIController()
{
	mPacmanIsAI = false;

	SetupGhosts(nullptr);
	SetupPacman(nullptr);
}

// ---------------------------------------------------------------------- //

AIController::AIController(char** collisionMapOfLevel)
{
	// Check if the player has chosen to play as pacman or not
	if (GameManager::Instance()->GetPlayerCharacterType() == PLAYER_CHARACTER_TYPE::PACMAN)
		mPacmanIsAI = false;
	else
		mPacmanIsAI = true;

	// Setup the ghosts and pacman
	SetupGhosts(collisionMapOfLevel);
	SetupPacman(collisionMapOfLevel);
}

// ---------------------------------------------------------------------- //

AIController::~AIController()
{
	// Clean up the player 
	if (mPacman)
	{
		delete mPacman;
		mPacman = nullptr;
	}

	// Clean the ghost memory up
	for (unsigned int i = 0; i < mGhosts.size(); i++)
	{
		if (mGhosts[i])
		{
			delete mGhosts[i];
			mGhosts[i] = nullptr;
		}
	}
	mGhosts.clear();
}

// ---------------------------------------------------------------------- //

void AIController::Render(const unsigned int framecount)
{
	// Render all ghosts
	for (unsigned int i = 0; i < mGhosts.size(); i++)
	{
		if (mGhosts[i])
			mGhosts[i]->Render();
	}

	if (mPacman)
		mPacman->Render(framecount);
}

// ---------------------------------------------------------------------- //

void AIController::Update(const float deltaTime)
{
	// Update all ghosts
	for (unsigned int i = 0; i < mGhosts.size(); i++)
	{
		// If the ghost exists
		if (mGhosts[i])
		{
			// If the ghost's state machine exists and there is at least one element in the stack
			if (mGhostStateMachines[i] && mGhostStateMachines[i]->PeekStack())
			{
				// Update the top of the stack
				mGhostStateMachines[i]->PeekStack()->OnUpdate();

				// Check if we should pop this task off of the stack
				mGhostStateMachines[i]->PeekStack()->CheckTransitions();
			}

			mGhosts[i]->Update(deltaTime, mPacman->GetCentrePosition());
		}
	}

	// Update pacman
	if (mPacman)
	{
		if (mPacmanIsAI)
		{
			// Check if we should pop this task off of the stack
			if (mPacmanStateMachine && mPacmanStateMachine->PeekStack())
			{
				mPacmanStateMachine->PeekStack()->CheckTransitions();

				// Pass in the current task to pacman to perform
				mPacman->UpdateAsAI(*(mPacmanStateMachine->PeekStack()));
			}


		}
		else
			mPacman->UpdateAsPlayerControlled(deltaTime);
	}
}

// ---------------------------------------------------------------------- //

void AIController::SetupGhosts(char** collisionMap)
{
	if (mGhosts.size() < NUMBER_OF_GHOSTS_IN_LEVEL)
	{
		// Create the ghosts
		for (unsigned int i = mGhosts.size(); i < NUMBER_OF_GHOSTS_IN_LEVEL; i++)
		{
			// First create the ghost - POSITIONING IS WRONG CURRENTLY
			mGhosts.push_back(new Ghost(S2D::Vector2(11.0f + float(i), 20.0f), collisionMap, (GHOST_TYPE)i, "Textures/Ghosts/Ghosts.png", 8, 4));

			// Now create the finite state machine that goes with this ghost - but only if the type the player is playing as is not the currently being created ghost type
			if(((int)GameManager::Instance()->GetPlayerCharacterType()) - 1 != i)
				mGhostStateMachines.push_back(new Stack_FiniteStateMachine_Ghost(true));
			else
				mGhostStateMachines.push_back(new Stack_FiniteStateMachine_Ghost(false)); // Still need a filler state machine so just disable it
		}
	}
}

// ---------------------------------------------------------------------- //

void AIController::SetupPacman(char** collisionMap)
{
	if (mPacman)
		delete mPacman;

	mPacman = new PacmanCharacter(collisionMap, 3, 3, mPacmanIsAI);


	// Now setup the pacman state machine
	if (mPacmanStateMachine)
		delete mPacmanStateMachine;

	if(mPacmanIsAI)
		mPacmanStateMachine = new Stack_FiniteStateMachine_Pacman();
}

// ---------------------------------------------------------------------- //