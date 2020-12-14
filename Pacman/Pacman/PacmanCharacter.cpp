#include "PacmanCharacter.h"

#include "Constants.h"
#include "GameManager.h"

#include "Stack_FiniteStateMachine.h"
#include "AudioManager.h"

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
	// Setup the state machine that is needed for the AI controls
	if (!mIsPlayerControlled)
		mStateMachine = new Stack_FiniteStateMachine_Pacman();
	else
		mStateMachine = nullptr;

	mCurrentFrame       = 8;
	mStartFrame         = 8;
	mEndFrame           = 8;

	mMovementSpeed      = PACMAN_MOVEMENT_SPEED;

	mFramesPerAnimation       = 6;
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
	mTargetPositon  = mStartPosition;

	mCurrentFacingDirection = FACING_DIRECTION::NONE;

	if (mStateMachine && !mIsPlayerControlled)
	{
		while (mStateMachine->PeekStack())
		{
			// Clear the state machine of anything that is on it
			mStateMachine->PopStack();
		}
	}

	// Now add the default behavior if is AI controlled
	if (!mIsPlayerControlled && mStateMachine)
		mStateMachine->PushToStack(PACMAN_STATE_TYPES::COLLECT_DOTS);
}

// ------------------------------------------------------------- //