#include "PacmanCharacter.h"

#include "Constants.h"
#include "GameManager.h"

#include "Stack_FiniteStateMachine.h"

#include <iostream>

// ------------------------------------------------------------- //

PacmanCharacter::PacmanCharacter(const char**       collisionMap, 
	                             const unsigned int spritesOnWidthMain, 
	                             const unsigned int spritesOnHeightMain, 
								 const unsigned int spritesOnWidthAlternate,
								 const unsigned int spritesOnHeightAlternate,
	                             const S2D::Vector2 startPositon, 
	                             const char*        filePathForMainSpriteSheet, 
	                             const char*        filePathForAlternateSpriteSheet, 
	                             bool               isAIControlled) : BaseCharacter(collisionMap,
													                                startPositon,
									                                                isAIControlled, 
									                                                filePathForMainSpriteSheet, 
									                                                filePathForAlternateSpriteSheet, 
									                                                spritesOnWidthMain, 
									                                                spritesOnHeightMain,
																					spritesOnWidthAlternate,
																					spritesOnHeightAlternate)
{
	// Setup the state machine that is needed for the AI controls
	if (!mIsPlayerControlled)
		mStateMachine = new Stack_FiniteStateMachine_Pacman();
	else
		mStateMachine = nullptr;

	mCurrentFrame = 0;
	mStartFrame   = 0;
	mEndFrame     = 1;

	mMovementSpeed = PACMAN_MOVEMENT_SPEED;

	mFramesPerAnimation = 6;
}

// ------------------------------------------------------------- //

PacmanCharacter::~PacmanCharacter()
{
	delete mMainSpriteSheet;
	mMainSpriteSheet = nullptr;

	delete mAlternateSpritesSheet;
	mAlternateSpritesSheet = nullptr;

	mCollisionMap = nullptr;
}

// ------------------------------------------------------------- //

void PacmanCharacter::CheckForDirectionChange()
{
	// Now check to see if the player can change direction 
	if (mRequestedFacingDirection == FACING_DIRECTION::NONE || mTimePerChangeDirectionRemaining > 0.0f)
	{
		// Quick out if the player has not entered anything
		return;
	}
	else
	{
		// If we have changed direction then reset the countdown for the next change of direction
		mTimePerChangeDirectionRemaining = PLAYER_CHANGE_DIRECTION_DELAY;

		switch (mRequestedFacingDirection)
		{
		case FACING_DIRECTION::DOWN:
			mStartFrame   = 6;
			mEndFrame     = 7;
			mCurrentFrame = 6;
		break;

		case FACING_DIRECTION::UP:
			mStartFrame   = 4;
			mEndFrame     = 5;
			mCurrentFrame = 4;
		break;

		case FACING_DIRECTION::LEFT:
			mStartFrame   = 2;
			mEndFrame     = 3;
			mCurrentFrame = 2;
		break;

		case FACING_DIRECTION::RIGHT:
			mStartFrame   = 0;
			mEndFrame     = 1;
			mCurrentFrame = 0;
		break;

		default:
			mStartFrame   = 8;
			mEndFrame     = 8;
			mCurrentFrame = 8;
		break;
		}

		// Set the new direction of facing
		mCurrentFacingDirection = mRequestedFacingDirection;
	}
}

// ------------------------------------------------------------- //

void PacmanCharacter::Update(const float deltaTime)
{
	if (mIsAlive)
	{
		// First call the base update function to do the generic update calls
		BaseCharacter::Update(deltaTime);

		if (mTimePerChangeDirectionRemaining > 0.0f)
			mTimePerChangeDirectionRemaining -= deltaTime;

		// Now move the player in the correct direction
		MoveInCurrentDirection(deltaTime);

		CheckForDirectionChange();

		// Now do this class specific update functionality
		if (mIsPlayerControlled)
			UpdateAsPlayerControlled(deltaTime);
		else
			UpdateAsAI();
	}
}

// ------------------------------------------------------------- //

void PacmanCharacter::UpdateAsPlayerControlled(const float deltaTime)
{
	// First check for input
	HandleInput();

	// Now we have the requested direction to face we need to check if it is a valid change, so:
	// Now check if the player should change facing direction
	CheckForDirectionChange();
}

// ------------------------------------------------------------- //

void PacmanCharacter::UpdateAsAI()
{
	if (mStateMachine->PeekStack())
	{
		mStateMachine->PeekStack()->OnUpdate();
		
		// Now perform the transition checks as to which state we should swap to
		mStateMachine->PeekStack()->CheckTransitions();
	}
}

// ------------------------------------------------------------- //

void PacmanCharacter::ReSetupPacmanSourceRect(float x, float y, unsigned int width, unsigned int height)
{
	mSourceRect = S2D::Rect(x, y, width, height);
}

// ------------------------------------------------------------- //

void PacmanCharacter::SetIsAlive(bool newVal)
{
	if (newVal)
		mIsAlive = newVal;
	else
	{
		if (mIsAlive)
		{
			mStartFrame   = 0;
			mEndFrame     = 11;
			mCurrentFrame = 0;

			mCurrentFacingDirection = FACING_DIRECTION::NONE;

			mIsAlive = newVal;
		}
	}
}

// ------------------------------------------------------------- //