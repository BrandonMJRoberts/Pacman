#include "PacmanCharacter.h"

#include "Constants.h"
#include "GameManager.h"

#include "Stack_FiniteStateMachine.h"
#include "AudioManager.h"
#include "DotsHandler.h"

#include <iostream>

// ------------------------------------------------------------- //

PacmanCharacter::PacmanCharacter(char** const       collisionMap, 
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
	// Setup the state machine that is needed for the AI controls - just disable it if the player is controlling pacman - to avoid any nullptr references
	mStateMachine = new Stack_FiniteStateMachine_Pacman(isAIControlled);

	mCurrentFrame       = 8;
	mStartFrame         = 8;
	mEndFrame           = 8;

	mMovementSpeed      = PACMAN_MOVEMENT_SPEED;

	mFramesPerAnimation       = 6;

	if (isAIControlled)
	{
		mMoveToPos = mCentrePosition;	

		mCurrentFacingDirection   = FACING_DIRECTION::NONE;
		mRequestedFacingDirection = FACING_DIRECTION::NONE;
	}
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
	if (mRequestedFacingDirection == FACING_DIRECTION::NONE || mRequestedFacingDirection == mCurrentFacingDirection || mTimePerChangeDirectionRemaining > 0.0f)
	{
		// Quick out if the player has not entered anything
		return;
	}
	else
	{
		// If we have changed direction then reset the countdown for the next change of direction
		if (mIsPlayerControlled)
			mTimePerChangeDirectionRemaining = PLAYER_CHANGE_DIRECTION_DELAY;
		else
			mTimePerChangeDirectionRemaining = 0.0f;

		switch (mRequestedFacingDirection)
		{
		case FACING_DIRECTION::DOWN:
			mStartFrame   = 6;
			mEndFrame     = 8;
			mCurrentFrame = 6;
		break;

		case FACING_DIRECTION::UP:
			mStartFrame   = 4;
			mEndFrame     = 6;
			mCurrentFrame = 4;
		break;

		case FACING_DIRECTION::LEFT:
			mStartFrame   = 2;
			mEndFrame     = 4;
			mCurrentFrame = 2;
		break;

		case FACING_DIRECTION::RIGHT:
			mStartFrame   = 0;
			mEndFrame     = 2;
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

void PacmanCharacter::Update(const float deltaTime, std::vector<S2D::Vector2> ghostPositions, DotsHandler& dotManager)
{
	if (mIsAlive)
	{
		BaseCharacter::Update(deltaTime);

		if (mTimePerChangeDirectionRemaining > 0.0f)
			mTimePerChangeDirectionRemaining -= deltaTime;

		// First check if pacman is player controlled
		if (mIsPlayerControlled)
		{
			MoveInCurrentDirection(deltaTime);

			CheckForDirectionChange();

			// Handle the player's input
			HandleInput();

			CheckForDirectionChange();
		}
		else
		{
			double accuracy = 0.3f;

			// First move in the current direction
			MoveInCurrentDirection(deltaTime);

			// Get the target position
			BaseState_Pacman* currentState = mStateMachine->PeekStack();
			if (currentState)
			{
				// Update the current state and get the target position for pacman
				currentState->OnUpdate(mCentrePosition, mTargetPositon, ghostPositions, dotManager);

				// Check if we should transition out of the current state
				currentState->CheckTransitions(*this, ghostPositions);
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
		if (AudioManager::GetInstance()->GetPacmanDeathSFXFinishedPlaying())
		{
			GameManager::Instance()->RestartLevel();
			AudioManager::GetInstance()->StopAllAudio();

			// Make sure everything is reset
			ResetPacmanFromDeath();
		}
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
			mEndFrame     = (mSpritesOnWidthAlternate * mSpritesOnHeightAlternate) - 1;
			mCurrentFrame = 0;

			mCurrentFacingDirection = FACING_DIRECTION::NONE;

			mIsAlive = newVal;
		}
	}
}

// ------------------------------------------------------------- //

void PacmanCharacter::Render(const unsigned int frameCount)
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

	// If enough frames have progressed then update the animation
	if (mFrameProgression > mFramesPerAnimation)
	{
		mFrameProgression -= mFramesPerAnimation;

		// Animations need to be dealt with differently if pacman has died
		if (mIsAlive)
		{
			// Increment the current frame
			mCurrentFrame++;

			// This is the pacman specific code - this is done to reduce the required size of the sprite sheet to save memory
			if (mCurrentFrame == mEndFrame)
			{
				if (mCurrentFrame != 8)
					mCurrentFrame = 8;
			}
			else if (mCurrentFrame > mEndFrame)
			{
				// Only loop if pacman is alive, as the death animation doesnt work if it loops				
				mCurrentFrame = mStartFrame;
			}
		}
		else
		{
			if (mCurrentFrame != mEndFrame)
			{
				mCurrentFrame++;
			}
		}
	}

	// First perform the checks to avoid crashing here
	if (mUsingMainSpriteSheet)
		if (!mMainSpriteSheet)
			return;
		else
			if (!mAlternateSpritesSheet)
				return;

	S2D::Vector2 mRenderPosition;

	if (mIsAlive)
	{
		// First calculate the source rect
		mSourceRect.X = float(   (mCurrentFrame % mSpritesOnWidthMain) * mSingleSpriteWidthMain);
		mSourceRect.Y = float(int(mCurrentFrame / mSpritesOnWidthMain) * mSingleSpriteHeightMain);

		mRenderPosition = S2D::Vector2(mCentrePosition.X - ((mSingleSpriteWidthMain * 0.5f) / SPRITE_RESOLUTION), mCentrePosition.Y - ((mSingleSpriteHeightMain * 0.5f) / SPRITE_RESOLUTION)) * SPRITE_RESOLUTION;

		// Now render the character in the correct position, 0.0 being the top left of the screen
		S2D::SpriteBatch::Draw(mMainSpriteSheet,
			                 &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
			                 &mSourceRect); // Draws Pacman
	}
	else
	{
		// First calculate the source rect
		mSourceRect.X = float(   (mCurrentFrame % mSpritesOnWidthAlternate) * mSingleSpriteWidthAlternate);
		mSourceRect.Y = float(int(mCurrentFrame / mSpritesOnWidthAlternate) * mSingleSpriteHeightAlternate);

		mRenderPosition = S2D::Vector2(mCentrePosition.X - ((mSingleSpriteWidthAlternate * 0.5f) / SPRITE_RESOLUTION), mCentrePosition.Y - ((mSingleSpriteHeightAlternate * 0.5f) / SPRITE_RESOLUTION)) * SPRITE_RESOLUTION;

		// Now render the character in the correct position, 0.0 being the top left of the screen
		S2D::SpriteBatch::Draw(mAlternateSpritesSheet,
			                  &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
			                  &mSourceRect); // Draws Pacman
	}
}

// ------------------------------------------------------------- //

void PacmanCharacter::ResetPacmanFromDeath()
{
	mIsAlive      = true;

	mCurrentFrame = 8;
	mEndFrame     = 8;
	mStartFrame   = 8;

	mCentrePosition = mStartPosition;
	mMoveToPos      = mCentrePosition;
	//mTargetPositon  = mStartPosition;

	mCurrentFacingDirection   = FACING_DIRECTION::NONE;
	mRequestedFacingDirection = FACING_DIRECTION::NONE;

	// Reset the state machine if there is one
	if (mStateMachine)
	{
		while (mStateMachine->PeekStack())
		{
			// Clear the state machine of anything that is on it
			mStateMachine->PopStack();
		}

		if(!mIsPlayerControlled)
			mStateMachine->PushToStack(PACMAN_STATE_TYPES::COLLECT_DOTS);
	}
}

// ------------------------------------------------------------- //