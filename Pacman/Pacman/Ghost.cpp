#include "Ghost.h"
#include "Constants.h"
#include "Commons.h"

#include "GameManager.h"

#include <iostream>

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

	mIsAlive                  = true;
	mGhostIsFleeing           = false;
	mGhostIsEaten             = false;

	// Create the state machine needed
	mStateMachine             = new Stack_FiniteStateMachine_Ghost(ghostType, isAIControlled);

	// Set the starting, ending and current frames
	switch (mThisGhostType)
	{
	case GHOST_TYPE::RED:
		mStartFrame   = 0;
		mEndFrame     = 1;
	break;

	case GHOST_TYPE::ORANGE:
		mStartFrame   = 24;
		mEndFrame     = 25;
	break;

	case GHOST_TYPE::PINK:
		mStartFrame   = 8;
		mEndFrame     = 9;
	break; 

	case GHOST_TYPE::BLUE:
		mStartFrame   = 16;
		mEndFrame     = 17;
	break;

	default:
	break;
	}

	mColourBaseEndFrame   = mEndFrame;
	mColourBaseStartFrame = mStartFrame;
	mCurrentFrame         = mStartFrame;

	mMovementSpeed        = GHOST_MOVEMENT_SPEED;

	mFramesPerAnimation   = 8;
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

	// First check if the ghost is player controlled
	if (mIsPlayerControlled)
	{
		// Handle the player's input to determine which direction the ghost should go
		HandleInput();
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
		mTimePerChangeDirectionRemaining = GHOST_CHANGE_DIRECTION_DELAY;

		switch (mRequestedFacingDirection)
		{
		case FACING_DIRECTION::DOWN:
			mStartFrame   = mColourBaseStartFrame + 6;
			mEndFrame     = mColourBaseEndFrame   + 6;
			mCurrentFrame = mColourBaseStartFrame + 6;
		break;

		case FACING_DIRECTION::UP:
			mStartFrame   = mColourBaseStartFrame + 4;
			mEndFrame     = mColourBaseEndFrame   + 4;
			mCurrentFrame = mColourBaseStartFrame + 4;
		break;

		case FACING_DIRECTION::LEFT:
			mStartFrame   = mColourBaseStartFrame + 2;
			mEndFrame     = mColourBaseEndFrame   + 2;
			mCurrentFrame = mColourBaseStartFrame + 2;
		break;

		case FACING_DIRECTION::RIGHT:
			mStartFrame   = mColourBaseStartFrame;
			mEndFrame     = mColourBaseEndFrame;
			mCurrentFrame = mColourBaseStartFrame;
		break;

		default:
			mStartFrame   = mColourBaseStartFrame;
			mEndFrame     = mColourBaseEndFrame;
			mCurrentFrame = mColourBaseStartFrame;
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

	// Check if we have arrived at the target position - if so then stop moving
	if (movementDifferential.LengthSquared() < checkingAccuracy)
	{
		mCurrentFacingDirection = FACING_DIRECTION::NONE;
		return;
	}

	// First calculate which movements are valid and then choose which one is the best for the current situation
	bool canMoveUp    = CanTurnToDirection(FACING_DIRECTION::UP);
	bool canMoveDown  = CanTurnToDirection(FACING_DIRECTION::DOWN);
	bool canMoveRight = CanTurnToDirection(FACING_DIRECTION::RIGHT);
	bool canMoveLeft  = CanTurnToDirection(FACING_DIRECTION::LEFT);

	// Quick exit if stuck in a wall somewhere
	if (!canMoveDown && !canMoveLeft && !canMoveRight && !canMoveLeft)
	{
		mRequestedFacingDirection = FACING_DIRECTION::NONE;
		return;
	}

	bool validXMove = false, validYMove = false;

	// First check if there is a movement that minimises the distance
	if ((movementDifferential.X > checkingAccuracy && canMoveRight) || (movementDifferential.X < -checkingAccuracy && canMoveLeft))
		validXMove = true;

	if ((movementDifferential.Y > checkingAccuracy && canMoveDown) || (movementDifferential.Y < -checkingAccuracy && canMoveUp))
		validYMove = true;

	// If both options are possible then move in the direction that reduces the distance overall
	if (validYMove && validXMove)
	{
		// If the difference in the X axis is more than the Y axis movement by such a degree then move in the x axis
		if (abs(movementDifferential.X) - abs(movementDifferential.Y) > 1.0f)
		{
			if (movementDifferential.X > checkingAccuracy)
			{
				mRequestedFacingDirection = FACING_DIRECTION::RIGHT;
			}
			else if (movementDifferential.X < -checkingAccuracy)
			{
				mRequestedFacingDirection = FACING_DIRECTION::LEFT;
			}

			return;
		}
		else // Otherwise always choose the Y axis
		{
			if (movementDifferential.Y > checkingAccuracy)
			{
				mRequestedFacingDirection = FACING_DIRECTION::DOWN;
			}
			else if (movementDifferential.Y < -checkingAccuracy)
			{
				mRequestedFacingDirection = FACING_DIRECTION::UP;
			}

			return;
		}
	}
	else if (validXMove || validYMove)
	{
		// If only one of the moves are valid then do that one
		if (validXMove)
		{
			if (mCurrentFacingDirection != FACING_DIRECTION::LEFT && movementDifferential.X > checkingAccuracy)
			{
				mRequestedFacingDirection = FACING_DIRECTION::RIGHT;
				return;
			}

			if (mCurrentFacingDirection != FACING_DIRECTION::RIGHT && movementDifferential.X < -checkingAccuracy)
			{
				mRequestedFacingDirection = FACING_DIRECTION::LEFT;
				return;
			}
		}
		else
		{
			if (mCurrentFacingDirection != FACING_DIRECTION::UP && movementDifferential.Y > checkingAccuracy)
			{
				mRequestedFacingDirection = FACING_DIRECTION::DOWN;
				return;
			}

			if (mCurrentFacingDirection != FACING_DIRECTION::DOWN && movementDifferential.Y < -checkingAccuracy)
			{
				mRequestedFacingDirection = FACING_DIRECTION::UP;
				return;
			}
		}
	}
	else
	{
		// If neither good option is avaliable then move away in the direction you are closest to pacman with
		if (abs(movementDifferential.X) < abs(movementDifferential.Y))
		{
			if (movementDifferential.X > 0)
				mRequestedFacingDirection = FACING_DIRECTION::LEFT;
			else
				mRequestedFacingDirection = FACING_DIRECTION::RIGHT;
		}
		else
		{
			if (movementDifferential.Y > 0)
				mRequestedFacingDirection = FACING_DIRECTION::UP;
			else
				mRequestedFacingDirection = FACING_DIRECTION::DOWN;
		}

		return;
	}

	return;
}

// ------------------------------------------------------------------------------------------------------------------------- //

void Ghost::Render(const unsigned int frameCount)
{
	// Make the frame loop in the way that has been set
	if ((frameCount % mFramesPerAnimation) == 1)
		mCurrentFrame++;

	if (mCurrentFrame > mEndFrame)
		mCurrentFrame = mStartFrame;

	// First perform the checks to avoid crashing here
	if (mUsingMainSpriteSheet)
		if (!mMainSpriteSheet)
			return;
		else
			if (!mAlternateSpritesSheet)
				return;

	// First calculate the source rect
	mSourceRect.X = float((mCurrentFrame % mSpritesOnWidth) * mSingleSpriteWidth);
	mSourceRect.Y = float(int(mCurrentFrame / mSpritesOnWidth) * mSingleSpriteHeight);

	// Now calculate the render position
	S2D::Vector2 mRenderPosition = S2D::Vector2(mCentrePosition.X - ((mSingleSpriteWidth * 0.5f) / SPRITE_RESOLUTION), mCentrePosition.Y - ((mSingleSpriteHeight * 0.5f) / SPRITE_RESOLUTION)) * SPRITE_RESOLUTION;

	if (mIsAlive && !mGhostIsFleeing && !mGhostIsEaten)
	{
		// Now render the character in the correct position, 0.0 being the top left of the screen
		S2D::SpriteBatch::Draw(mMainSpriteSheet,
			                 &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
			                 &mSourceRect); // Draws Pacman
	}
	else if (!mIsAlive || mGhostIsFleeing || mGhostIsEaten)
	{
		// Now render the character in the correct position, 0.0 being the top left of the screen
		S2D::SpriteBatch::Draw(mAlternateSpritesSheet,
			                 &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
			                 &mSourceRect); // Draws Pacman
	}
}

// ------------------------------------------------------------------------------------------------------------------------- //