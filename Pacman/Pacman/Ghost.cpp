#include "Ghost.h"
#include "Constants.h"
#include "Commons.h"

#include "GameManager.h"
#include "AudioManager.h"

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
			 const unsigned int spritesOnHeightAlternate, 
	         const bool         startHome) : BaseCharacter(collisionMap, startPos, isAIControlled, filePathForMainSpriteSheet, filePathForAlternateSpriteSheet, spritesOnWidthMain, spritesOnHeightMain, spritesOnWidthAlternate, spritesOnHeightAlternate)
{
	mThisGhostType            = ghostType;

	mIsAlive                  = true; // Default alive
	mIsHome                   = startHome;
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

	// If the ghosts starts in their home then they cannot leave by default, but if they dont start at their home then they can leave by default
	mStartAtHome = startHome;
	if (startHome)
	{
		mCanLeaveHome = false;

		if(mStateMachine)
			mStateMachine->PushToStack(GHOST_STATE_TYPE::EXIT_HOME);
	}
	else
		mCanLeaveHome = true;

	mResetting             = false;
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
	if (!mResetting)
	{
		// First call the base class update
		BaseCharacter::Update(deltaTime);

		if (mTimePerChangeDirectionRemaining > 0.0f)
			mTimePerChangeDirectionRemaining -= deltaTime;

		// First check if the ghost is player controlled
		if (mIsPlayerControlled)
		{
			// As the transition into fleeing is an AI controlled thing we need to do it if the ghost is player controlled
			if (GameManager::Instance()->GetIsPlayerPoweredUp() && !mGhostIsFleeing)
				SetGhostIsFleeing(true);
			else if (!GameManager::Instance()->GetIsPlayerPoweredUp() && mGhostIsFleeing)
				SetGhostIsFleeing(false);

			MoveInCurrentDirection(deltaTime);

			CheckForDirectionChange();

			// Handle the player's input to determine which direction the ghost should go
			HandleInput();

			CheckForDirectionChange();
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
			if (S2D::Vector2::Distance(mCentrePosition, mMoveToPos) < accuracy)
			{
				CalculateAIMovementDirection(); // Now calclate where we need to actually move to - the centre of which segment
			}

			// Check if the ghost should change facing direction
			CheckForDirectionChange();
		}
	}
	else
	{ 
		// Take time off the delay

		if (AudioManager::GetInstance()->GetPacmanDeathSFXFinishedPlaying())
		{
			mResetting = false;

			ResetGhostFromDeath();
		}
	}
}

// -------------------------------------------------------------------- //

void Ghost::CheckForDirectionChange()
{
	// Now check to see if the player can change direction 
	if (mRequestedFacingDirection == FACING_DIRECTION::NONE || mRequestedFacingDirection == mCurrentFacingDirection || mTimePerChangeDirectionRemaining > 0.0f)
	{
		// Quick out if the player has not entered anything
		return;
	}
	else
	{
		// If we have changed direction then reset the countdown for the next change of direction
		if (mIsPlayerControlled)
			mTimePerChangeDirectionRemaining = GHOST_CHANGE_DIRECTION_DELAY;
		else
			mTimePerChangeDirectionRemaining = 0.0f;

		if (mIsHome && !mCanLeaveHome)
			return;

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
		mSourceRect.X   = float(   (mCurrentFrame % mSpritesOnWidthAlternate) * mSingleSpriteWidthAlternate);
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
	{
		AudioManager::GetInstance()->PauseGhostGoingToHomeSFX();
		return;
	}
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

		// Also play the sfx for the ghosts going to their home
		AudioManager::GetInstance()->PlayGhostGoingToHomeSFX();
	}
}

// ------------------------------------------------------------------------------------------------------------------------- //

void Ghost::ResetGhostFromDeath()
{
	// Reset the position
	mCentrePosition         = mStartPosition;

	// Reset the facing direction
	mCurrentFacingDirection = FACING_DIRECTION::NONE;

	// Set the starting, ending and current frames
	switch (mThisGhostType)
	{
	case GHOST_TYPE::RED:
		mStartFrame = 0;
		mEndFrame   = 1;
	break;

	case GHOST_TYPE::ORANGE:
		mStartFrame = 24;
		mEndFrame   = 25;
	break;

	case GHOST_TYPE::PINK:
		mStartFrame = 8;
		mEndFrame   = 9;
	break;

	case GHOST_TYPE::BLUE:
		mStartFrame = 16;
		mEndFrame   = 17;
	break;

	default: break;
	}

	mColourBaseEndFrame   = mEndFrame;
	mColourBaseStartFrame = mStartFrame;
	mCurrentFrame         = mStartFrame;
	
	// Reset the movement speed
	mMovementSpeed  = GHOST_MOVEMENT_SPEED;

	mIsAlive	    = true;
	mIsHome         = mStartAtHome;
	mGhostIsFleeing = false;
	mGhostIsEaten   = false;
	mCanLeaveHome   = !mStartAtHome;

	// If it is AI controlled then reset the state machine
	if (!mIsPlayerControlled && mStateMachine)
	{
		while (mStateMachine->PeekStack())
			mStateMachine->PopStack();

		mStateMachine->PushToStack(GHOST_STATE_TYPE::CHASE);

		mMoveToPos = mCentrePosition;

		if (mStartAtHome)
			mStateMachine->PushToStack(GHOST_STATE_TYPE::EXIT_HOME);
	}
}

// ------------------------------------------------------------------------------------------------------------------------- //

void Ghost::SetGhostsShouldReset()
{
	mResetting                = true;
	mCurrentFacingDirection   = FACING_DIRECTION::NONE;
	mRequestedFacingDirection = FACING_DIRECTION::NONE;
}

// ------------------------------------------------------------------------------------------------------------------------- //

bool Ghost::CanTurnToDirection(const FACING_DIRECTION newDir)
{
	S2D::Vector2 offset = S2D::Vector2(0.0f, 0.0f);

	switch (newDir)
	{
	case FACING_DIRECTION::UP:
		offset.X = 0.5f;
		offset.Y = -0.5f;
	break;

	case FACING_DIRECTION::DOWN:
		offset.X = 0.5f;
		offset.Y = 1.5f;
	break;

	case FACING_DIRECTION::LEFT:
		offset.X = -0.5f;
		offset.Y = 0.5f;
	break;

	case FACING_DIRECTION::RIGHT:
		offset.X = 1.5f;
		offset.Y = 0.5f;
	break;
	}

	// Check if the position is open, or if we are in certain states then we can move through collision marked with a '2'
	if (mCollisionMap[(unsigned int)(int(mCentrePosition.Y) + offset.Y)][(unsigned int)(int(mCentrePosition.X) + offset.X)] == '0' || 
		(mCollisionMap[(unsigned int)(int(mCentrePosition.Y) + offset.Y)][(unsigned int)(int(mCentrePosition.X) + offset.X)] == '2' && (mStateMachine->PeekStack()->GetType() == GHOST_STATE_TYPE::EXIT_HOME || mStateMachine->PeekStack()->GetType() == GHOST_STATE_TYPE::RETURN_HOME)))
	{
		return true;
	}

	return false;
}

// ------------------------------------------------------------------------------------------------------------------------- //