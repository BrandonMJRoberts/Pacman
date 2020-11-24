#include "Ghost.h"
#include "Constants.h"
#include "Commons.h"

#include "GameManager.h"

#include <iostream>

//S2D::Texture2D* Ghost::mColouredSpriteSheet = nullptr;
//S2D::Texture2D* Ghost::mFleeSpriteSheet = nullptr;

//S2D::Rect       Ghost::mSourceRect  = S2D::Rect();

//unsigned int    Ghost::mSingleSpriteWidth  = 0;
//unsigned int    Ghost::mSingleSpriteHeight = 0;
//char**          Ghost::mCollisionMap = nullptr;

// -------------------------------------------------------------------- //

Ghost::Ghost(const S2D::Vector2 startPos, 
	         const char**       collisionMap, 
	         const GHOST_TYPE   ghostType, 
	         const bool         isAIControlled, 
	         const char*        filePathForMainSpriteSheet, 
	         const char*        filePathForAlternateSpriteSheet, 
	         const unsigned int spritesOnWidth, 
	         const unsigned int spritesOnHeight) : BaseCharacter(collisionMap, startPos, isAIControlled, filePathForMainSpriteSheet, filePathForAlternateSpriteSheet, spritesOnWidth, spritesOnHeight)
{
	mThisGhostType            = ghostType;

	//mIsGhostAlive             = true;
	mGhostIsFleeing           = false;

	// Create the state machine needed
	mStateMachine             = new Stack_FiniteStateMachine_Ghost(ghostType, isAIControlled);

	// Set the starting, ending and current frames
	switch (mThisGhostType)
	{
	case GHOST_TYPE::RED:
		mCurrentFrame = 0;
		mStartFrame   = 0;
		mEndFrame     = 1;
	break;

	case GHOST_TYPE::ORANGE:
		mCurrentFrame = 24;
		mStartFrame   = 24;
		mEndFrame     = 25;
	break;

	case GHOST_TYPE::PINK:
		mCurrentFrame = 8;
		mStartFrame   = 8;
		mEndFrame     = 9;
	break; 

	case GHOST_TYPE::BLUE:
		mCurrentFrame = 16;
		mStartFrame   = 16;
		mEndFrame     = 17;
	break;

	default:
	break;
	}

	mMovementSpeed = GHOST_MOVEMENT_SPEED;
}

// -------------------------------------------------------------------- //

Ghost::~Ghost()
{
	// Clear up the memory used
	delete mMainSpriteSheet;
	mMainSpriteSheet = nullptr;

	delete mAlternateSpritesSheet;
	mAlternateSpritesSheet = nullptr;

	mCollisionMap = nullptr;
}

// -------------------------------------------------------------------- //

void Ghost::Update(const float deltaTime, const S2D::Vector2 pacmanPos, const FACING_DIRECTION pacmanFacingDirection)
{
	// First call the base class update
	BaseCharacter::Update(deltaTime);

	// First the ghost's movemt 
	if (mIsPlayerControlled)
	{
		// Handle the player's input to determine which direction the ghost should go

	}
	else
	{
		// Otherwise have the AI make the decision.
		BaseState_Ghost* currentState = mStateMachine->PeekStack();
		if (currentState)
		{
			if (mThisGhostType != GHOST_TYPE::ORANGE)
			{
				// Update the current state  
				currentState->OnUpdate(mTargetPositon, pacmanPos, pacmanFacingDirection);
			}
			else if (mThisGhostType == GHOST_TYPE::ORANGE && mCentrePosition == mTargetPositon)
			{
				currentState->OnUpdate(mTargetPositon, pacmanPos, pacmanFacingDirection);
			}

			// Check if we should transition out of the current state
			currentState->CheckTransitions(this);
		}

		mTimePerChangeDirectionRemaining -= deltaTime;

		if (mTimePerChangeDirectionRemaining <= 0.0f)
		{
			// Now calculate how we are going to get to where we want to go
			CalculateAIMovementDirection();

			mTimePerChangeDirectionRemaining = mTimePerChangeOfDirection;
		}
	}
}

// -------------------------------------------------------------------- //

void Ghost::CheckForDirectionChange()
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
			mStartFrame = 6;
			mEndFrame = 8;
			mCurrentFrame = 6;
		break;

		case FACING_DIRECTION::UP:
			mStartFrame = 4;
			mEndFrame = 6;
			mCurrentFrame = 4;
		break;

		case FACING_DIRECTION::LEFT:
			mStartFrame = 2;
			mEndFrame = 4;
			mCurrentFrame = 2;
		break;

		case FACING_DIRECTION::RIGHT:
			mStartFrame = 0;
			mEndFrame = 2;
			mCurrentFrame = 0;
		break;

		default:
			mStartFrame = 8;
			mEndFrame = 8;
			mCurrentFrame = 8;
		break;
		}

		// Set the new direction of facing
		mCurrentFacingDirection = mRequestedFacingDirection;
	}
}

// -------------------------------------------------------------------- //

void Ghost::CalculateAIMovementDirection()
{
	// Now calculate where we need to go to get to the taget position
	S2D::Vector2 movementDifferential = mTargetPositon - mCentrePosition;

	float checkingAccuracy            = 0.5f;

	// Check if we have arrived at the target position - if so then stop moveing
	if (movementDifferential.X <= checkingAccuracy && movementDifferential.X > -checkingAccuracy && movementDifferential.Y <= checkingAccuracy && movementDifferential.Y > -checkingAccuracy)
	{
		mCurrentFacingDirection = FACING_DIRECTION::NONE;
		return;
	}

	// First calculate which movements are valid and then choose which one is the best for the current situation
	bool canMoveUp    = CanTurnToDirection(FACING_DIRECTION::UP);
	bool canMoveDown  = CanTurnToDirection(FACING_DIRECTION::DOWN);
	bool canMoveRight = CanTurnToDirection(FACING_DIRECTION::RIGHT);
	bool canMoveLeft  = CanTurnToDirection(FACING_DIRECTION::LEFT);

	bool validVerticalMove   = false;
	bool validHorizontalMove = false;

	// Now check if the directions you want to go are valid moves
	if (movementDifferential.Y > checkingAccuracy && canMoveDown)
		validVerticalMove = true;
	else if (movementDifferential.Y < -checkingAccuracy && canMoveUp)
		validVerticalMove = true;

	if (movementDifferential.X > checkingAccuracy && canMoveRight)
		validHorizontalMove = true;
	else if (movementDifferential.X < -checkingAccuracy && canMoveLeft)
		validHorizontalMove = true;

	// If you have two valid axis of movement then choose the axis that has the largest distance remaining
	if (validHorizontalMove && validVerticalMove)
	{
		// Then choose the option that minimses the distance
		if (abs(movementDifferential.X) > abs(movementDifferential.Y))
		{
			if (movementDifferential.X > checkingAccuracy && mCurrentFacingDirection != FACING_DIRECTION::LEFT)
			{
				mCurrentFacingDirection = FACING_DIRECTION::RIGHT;
			}
			else if (movementDifferential.X < -checkingAccuracy && mCurrentFacingDirection != FACING_DIRECTION::RIGHT)
			{
				mCurrentFacingDirection = FACING_DIRECTION::LEFT;
			}
		}
		else
		{
			if (movementDifferential.Y > checkingAccuracy && mCurrentFacingDirection != FACING_DIRECTION::UP)
			{
				mCurrentFacingDirection = FACING_DIRECTION::DOWN;
			}
			else if (movementDifferential.Y < -checkingAccuracy && mCurrentFacingDirection != FACING_DIRECTION::DOWN)
			{
				mCurrentFacingDirection = FACING_DIRECTION::UP;
			}
		}
	}
	else if (validHorizontalMove || validVerticalMove) // If both options are not valid, but one of them is then move in the valid direction
	{
		if (movementDifferential.X > checkingAccuracy && canMoveRight && mCurrentFacingDirection != FACING_DIRECTION::LEFT)
			mCurrentFacingDirection = FACING_DIRECTION::RIGHT;
		else if (movementDifferential.X < -checkingAccuracy && canMoveLeft && mCurrentFacingDirection != FACING_DIRECTION::RIGHT)
			mCurrentFacingDirection = FACING_DIRECTION::LEFT;
		else if (movementDifferential.Y > checkingAccuracy && canMoveDown && mCurrentFacingDirection != FACING_DIRECTION::UP)
			mCurrentFacingDirection = FACING_DIRECTION::DOWN;
		else if (movementDifferential.Y < -checkingAccuracy && canMoveUp && mCurrentFacingDirection != FACING_DIRECTION::DOWN)
			mCurrentFacingDirection = FACING_DIRECTION::UP;
	}
	else // If neither of the desired moves are valid then just move in a valid direction
	{
		if (canMoveDown && mCurrentFacingDirection != FACING_DIRECTION::UP)
			mCurrentFacingDirection = FACING_DIRECTION::DOWN;
		else if (canMoveUp && mCurrentFacingDirection != FACING_DIRECTION::DOWN)
			mCurrentFacingDirection = FACING_DIRECTION::UP;
		else if (canMoveLeft && mCurrentFacingDirection != FACING_DIRECTION::RIGHT)
			mCurrentFacingDirection = FACING_DIRECTION::LEFT;
		else if (canMoveRight && mCurrentFacingDirection != FACING_DIRECTION::LEFT)
			mCurrentFacingDirection = FACING_DIRECTION::RIGHT;
	}
}