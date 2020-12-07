#include "Ghost.h"
#include "Constants.h"
#include "Commons.h"

#include "GameManager.h"

#include <iostream>

// -------------------------------------------------------------------- //

Ghost::Ghost(const S2D::Vector2 startPos, 
	         char** const       collisionMap, 
	         const GHOST_TYPE   ghostType, 
	         const bool         isAIControlled, 
	         const char*        filePathForMainSpriteSheet, 
	         const char*        filePathForAlternateSpriteSheet, 
	         const unsigned int spritesOnWidthMain, 
	         const unsigned int spritesOnHeightMain, 
			 const unsigned int spritesOnWidthAlternate,
			 const unsigned int spritesOnHeightAlternate) : BaseCharacter(collisionMap, startPos, isAIControlled, filePathForMainSpriteSheet, filePathForAlternateSpriteSheet, spritesOnWidthMain, spritesOnHeightMain, spritesOnWidthAlternate, spritesOnHeightAlternate)
{
	mThisGhostType            = ghostType;

	mIsAlive                  = true;
	mIsHome                   = false;
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

	mColourBaseEndFrame       = mEndFrame;
	mColourBaseStartFrame     = mStartFrame;
	mCurrentFrame             = mStartFrame;

	mMovementSpeed			  = GHOST_MOVEMENT_SPEED;

	mFramesPerAnimation       = 10;
	mDoorIsOpen               = false;
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
		double accuracy = 0.3f;

		// First move in the current direction
		MoveInCurrentDirection(deltaTime);

		// Get the target position - normally where pacman is or where to flee to
		BaseState_Ghost* currentState = mStateMachine->PeekStack();
		if (currentState)
		{
			// Update the current state and get the target position for this ghost
			currentState->OnUpdate(mTargetPositon, pacmanPos, pacmanFacingDirection);
		
			// Check if we should transition out of the current state
			currentState->CheckTransitions(this);
		}

		// If we are at the next position to be moved to then calculate another one
	    if(S2D::Vector2::Distance(mCentrePosition, mMoveToPos) < accuracy)
		{
			CalculateAIMovementDirection(); // Now calclate where we need to actually move to - the centre of which segment
			CheckForDirectionChange();      // Check if the ghost should change facing direction
		}
	}
}

// -------------------------------------------------------------------- //

void Ghost::CheckForDirectionChange()
{
	// Now check to see if the player can change direction 
	if (mRequestedFacingDirection == FACING_DIRECTION::NONE)
	{
		// Quick out if the player has not entered anything
		return;
	}
	else
	{
		// If we have changed direction then reset the countdown for the next change of direction
		mTimePerChangeDirectionRemaining = GHOST_CHANGE_DIRECTION_DELAY;

		if (mIsAlive && !mGhostIsEaten && !mGhostIsFleeing)
		{
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
		}
		else if (mGhostIsEaten)
		{
			switch (mRequestedFacingDirection)
			{
			case FACING_DIRECTION::DOWN:
				mStartFrame   = 7;
				mEndFrame     = 7;
				mCurrentFrame = 7;
			break;

			case FACING_DIRECTION::UP:
				mStartFrame   = 6;
				mEndFrame     = 6;
				mCurrentFrame = 6;
			break;

			case FACING_DIRECTION::LEFT:
				mStartFrame   = 5;
				mEndFrame     = 5;
				mCurrentFrame = 5;
			break;

			case FACING_DIRECTION::RIGHT:
				mStartFrame   = 4;
				mEndFrame     = 4;
				mCurrentFrame = 4;
			break;

			default:
				mStartFrame   = 4;
				mEndFrame     = 4;
				mCurrentFrame = 4;
			break;
			}
		}
		else if (mGhostIsFleeing && !mGhostIsEaten)
		{
			mStartFrame   = 0;
			mEndFrame     = 1;
			mCurrentFrame = 0;
		}

		// Set the new direction of facing
		mCurrentFacingDirection = mRequestedFacingDirection;
	}
}

// -------------------------------------------------------------------- //

void Ghost::SetToMoveInDirection(FACING_DIRECTION direction)
{
	switch (direction)
	{
	case FACING_DIRECTION::RIGHT:
		// Set the move to pos to be the centre of the next segment to move to
		mMoveToPos                = S2D::Vector2((int)mCentrePosition.X + 1.5f, (int)mCentrePosition.Y + 0.5f);
		mRequestedFacingDirection = FACING_DIRECTION::RIGHT;
	break;

	case FACING_DIRECTION::LEFT:
		// Set the move to pos to be the centre of the next segment to move to
		mMoveToPos                = S2D::Vector2((int)mCentrePosition.X - 0.5f, (int)mCentrePosition.Y + 0.5f);
		mRequestedFacingDirection = FACING_DIRECTION::LEFT;
	break;

	case FACING_DIRECTION::UP:
		// Set the move to pos to be the centre of the next segment to move to
		mMoveToPos                = S2D::Vector2((int)mCentrePosition.X + 0.5f, (int)mCentrePosition.Y - 0.5f);
		mRequestedFacingDirection = FACING_DIRECTION::UP;
	break;

	case FACING_DIRECTION::DOWN:
		// Set the move to pos to be the centre of the next segment to move to
		mMoveToPos                = S2D::Vector2((int)mCentrePosition.X + 0.5f, (int)mCentrePosition.Y + 1.5f);
		mRequestedFacingDirection = FACING_DIRECTION::DOWN;
	break;
	}
}

// -------------------------------------------------------------------- //

void Ghost::CalculateAIMovementDirection()
{
	// Now calculate where we need to go to get to the taget position
	S2D::Vector2 movementDifferential = mTargetPositon - mCentrePosition;

	// First calculate which movements are valid and then choose which one is the best for the current situation
	bool canMoveUp    = CanTurnToDirection(FACING_DIRECTION::UP)    && mCurrentFacingDirection != FACING_DIRECTION::DOWN;
	bool canMoveDown  = CanTurnToDirection(FACING_DIRECTION::DOWN)  && mCurrentFacingDirection != FACING_DIRECTION::UP;
	bool canMoveRight = CanTurnToDirection(FACING_DIRECTION::RIGHT) && mCurrentFacingDirection != FACING_DIRECTION::LEFT;
	bool canMoveLeft  = CanTurnToDirection(FACING_DIRECTION::LEFT)  && mCurrentFacingDirection != FACING_DIRECTION::RIGHT;

	// So that you only consider the alternate axis if beyond a certain amount
	float accuracy = 0.5f;

	bool validVertical = false, validHorizontal = false;

	if ((movementDifferential.X > accuracy && canMoveRight) || 
		(movementDifferential.X < -accuracy && canMoveLeft))
		validHorizontal = true;

	if ((movementDifferential.Y > accuracy && canMoveDown) || 
		(movementDifferential.Y < -accuracy && canMoveUp))
		validVertical = true;

	// If both axis are valid then choose the one with the greatest distance to go
	if (validVertical && validHorizontal)
	{
		// Choose the direction with the greatest distance remaining
		if (abs(movementDifferential.X) > abs(movementDifferential.Y))
		{
			// If you want to go right and you can, then go right
			if (canMoveRight && movementDifferential.X > accuracy)
				SetToMoveInDirection(FACING_DIRECTION::RIGHT);

			if (canMoveLeft && movementDifferential.X < -accuracy)
				SetToMoveInDirection(FACING_DIRECTION::LEFT);

			return;
		}
		else
		{
			if (canMoveDown && movementDifferential.Y > accuracy)
				SetToMoveInDirection(FACING_DIRECTION::DOWN);

			if (canMoveUp && movementDifferential.Y < -accuracy)
				SetToMoveInDirection(FACING_DIRECTION::UP);

			return;
		}
	}
	else if (validVertical || validHorizontal)
	{
		// If only one option is avaliable then go in that direction
		if (canMoveRight && movementDifferential.X > accuracy)
			SetToMoveInDirection(FACING_DIRECTION::RIGHT);

		if (canMoveLeft && movementDifferential.X < -accuracy)
			SetToMoveInDirection(FACING_DIRECTION::LEFT);

		if (canMoveDown && movementDifferential.Y > accuracy)
			SetToMoveInDirection(FACING_DIRECTION::DOWN);

		if (canMoveUp && movementDifferential.Y < -accuracy)
			SetToMoveInDirection(FACING_DIRECTION::UP);

		return;
	}
	else
	{
		// If there are no good options then just choose another direction to go
		if (canMoveDown)
			SetToMoveInDirection(FACING_DIRECTION::DOWN);
		else if (canMoveUp)
			SetToMoveInDirection(FACING_DIRECTION::UP);
		else if (canMoveLeft)
			SetToMoveInDirection(FACING_DIRECTION::LEFT);
		else if (canMoveRight)
			SetToMoveInDirection(FACING_DIRECTION::RIGHT);

		return;
	}

	return;
}

// ------------------------------------------------------------------------------------------------------------------------- //

void Ghost::Render(const unsigned int frameCount)
{
	// First calculate the amount of frames that have passed since the last run through
	int deltaFrames;

	// we must have looped around the FPS limit higher up, so account for this
	if (frameCount < mPriorFrameCount)
		deltaFrames = (FRAME_RATE - mPriorFrameCount) + frameCount; // Set the frame progression to be the remainder required to hit the FPS limit, and then add the current frame count
	else
		deltaFrames = frameCount - mPriorFrameCount;

	// Add these frames onto the count
	mFrameProgression += deltaFrames;

	mPriorFrameCount = frameCount;

	if (mFrameProgression > mFramesPerAnimation)
	{
		mFrameProgression -= mFramesPerAnimation;

		mCurrentFrame++;

		if (mCurrentFrame > mEndFrame)
			mCurrentFrame = mStartFrame;
	}

	// First perform the checks to avoid crashing here
	if (mUsingMainSpriteSheet)
		if (!mMainSpriteSheet)
			return;
		else
			if (!mAlternateSpritesSheet)
				return;

	// Now calculate the render position
	S2D::Vector2 mRenderPosition;

	if (mIsAlive && !mGhostIsFleeing && !mGhostIsEaten)
	{
		// First calculate the source rect
		mSourceRect.X   = float((mCurrentFrame % mSpritesOnWidthMain) * mSingleSpriteWidthMain);
		mSourceRect.Y   = float(int(mCurrentFrame / mSpritesOnWidthMain) * mSingleSpriteHeightMain);

		mRenderPosition = S2D::Vector2(mCentrePosition.X - ((mSingleSpriteWidthMain * 0.5f) / SPRITE_RESOLUTION), mCentrePosition.Y - ((mSingleSpriteHeightMain * 0.5f) / SPRITE_RESOLUTION)) * SPRITE_RESOLUTION;

		// Now render the character in the correct position, 0.0 being the top left of the screen
		S2D::SpriteBatch::Draw(mMainSpriteSheet,
			                 &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
			                 &mSourceRect); // Draws Pacman
	}
	else if (!mIsAlive || mGhostIsFleeing || mGhostIsEaten)
	{
		// First calculate the source rect
		mSourceRect.X   = float((mCurrentFrame % mSpritesOnWidthAlternate) * mSingleSpriteWidthAlternate);
		mSourceRect.Y   = float(int(mCurrentFrame / mSpritesOnWidthAlternate) * mSingleSpriteHeightAlternate);

		mRenderPosition = S2D::Vector2(mCentrePosition.X - ((mSingleSpriteWidthAlternate * 0.5f) / SPRITE_RESOLUTION), mCentrePosition.Y - ((mSingleSpriteHeightAlternate * 0.5f) / SPRITE_RESOLUTION)) * SPRITE_RESOLUTION;

		// Now render the character in the correct position, 0.0 being the top left of the screen
		S2D::SpriteBatch::Draw(mAlternateSpritesSheet,
			                 &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
			                 &mSourceRect); // Draws Pacman
	}
}

// ------------------------------------------------------------------------------------------------------------------------- //

void Ghost::SetGhostIsFleeing(bool newVal)
{
	mGhostIsFleeing = newVal;

	if (!newVal)
		return;
	else
	{
		mStartFrame   = 0;
		mEndFrame     = 1;
		mCurrentFrame = 1;
	}
}

// ------------------------------------------------------------------------------------------------------------------------- //

void Ghost::SetGhostIsEaten(bool newVal)
{
	mGhostIsEaten = newVal;

	if (!newVal)
		return;
	else
	{
		switch (mCurrentFacingDirection)
		{
		case FACING_DIRECTION::DOWN:
			mStartFrame = 7;
		break;

		case FACING_DIRECTION::UP:
			mStartFrame = 6;
		break;

		case FACING_DIRECTION::LEFT:
			mStartFrame = 5;
		break;

		case FACING_DIRECTION::RIGHT:
			mStartFrame = 4;
		break;

		default:
			mStartFrame = 4;
		break;
		}

		mEndFrame     = mStartFrame;
		mCurrentFrame = mStartFrame;
	}
}

// ------------------------------------------------------------------------------------------------------------------------- //

void Ghost::ToggleDoorToHome()
{
	// First convert the collision map into a non-const array and then change the door to be open/closed
	if (mDoorIsOpen)
	{
		mCollisionMap[14][12] = '1';
	}
	else
	{
		mCollisionMap[14][12] = '0';
	}

	mDoorIsOpen = !mDoorIsOpen;
}

// ------------------------------------------------------------------------------------------------------------------------- //