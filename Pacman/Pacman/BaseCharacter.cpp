#include "BaseCharacter.h"

#include "Constants.h"
#include "GameManager.h"

#include <iostream>

// -------------------------------------------------------------------------------- //

BaseCharacter::BaseCharacter(char**             collisionMap, 
	                         const S2D::Vector2 startPosition, 
	                         const bool         isAIControlled, 
	                         const char*        filePathForMainSpriteSheet, 
	                         const char*        filePathForAlternateSpriteSheet, 
	                         const unsigned int spritesOnWidthMain, 
	                         const unsigned int spritesOnHeightMain,
							 const unsigned int spritesOnWidthAlternate,
							 const unsigned int spritesOnHeightAlternate) : mSpritesOnHeightMain(spritesOnHeightMain),
																			mSpritesOnWidthMain(spritesOnWidthMain),
																			mSpritesOnHeightAlternate(spritesOnHeightAlternate),
																			mSpritesOnWidthAlternate(spritesOnWidthAlternate),
																			mTimePerChangeOfDirection(PLAYER_CHANGE_DIRECTION_DELAY)
{
	// Load in the sprite sheets
	mMainSpriteSheet = new S2D::Texture2D();
	mMainSpriteSheet->Load(filePathForMainSpriteSheet, false);
	if (!mMainSpriteSheet)
	{
		std::cout << "Failed to load in the main sprite sheet for a character!" << std::endl;
		return;
	}

	mAlternateSpritesSheet = new S2D::Texture2D();
	mAlternateSpritesSheet->Load(filePathForAlternateSpriteSheet, false);
	if (!mAlternateSpritesSheet)
	{
		std::cout << "Failed to load in the main sprite sheet for a character!" << std::endl;
		return;
	}

	// Calculate the single sprite width and height
	mSingleSpriteHeightMain              = mMainSpriteSheet->GetHeight() / spritesOnHeightMain;
	mSingleSpriteWidthMain               = mMainSpriteSheet->GetWidth() / spritesOnWidthMain;

	mSingleSpriteHeightAlternate         = mAlternateSpritesSheet->GetHeight() / spritesOnHeightAlternate;
	mSingleSpriteWidthAlternate          = mAlternateSpritesSheet->GetWidth() / spritesOnWidthAlternate;

	// Set if the character is AI controlled
	mIsPlayerControlled              = !isAIControlled;

	// Store the start position
	mStartPosition                   = startPosition;
	mCentrePosition                  = startPosition;

	// Set the default values
	mTimePerChangeDirectionRemaining = mTimePerChangeOfDirection;
	mUsingMainSpriteSheet            = true;
	mMovementSpeed                   = 0.0f;

	mSourceRect.Width                = mSingleSpriteWidthMain;
	mSourceRect.Height               = mSingleSpriteHeightMain;

	mCurrentFacingDirection          = FACING_DIRECTION::NONE;
	mRequestedFacingDirection        = FACING_DIRECTION::NONE;

	mFramesPerAnimation              = 0;
	mPriorFrameCount                 = 0;
	mMoveToPos                       = mCentrePosition;

	mCollisionMap                    = collisionMap;
	mIsAlive                         = true;
}

// -------------------------------------------------------------------------------- //

BaseCharacter::~BaseCharacter()
{
	delete mMainSpriteSheet;
	mMainSpriteSheet = nullptr;

	delete mAlternateSpritesSheet;
	mAlternateSpritesSheet = nullptr;

	mCollisionMap = nullptr;
}

// -------------------------------------------------------------------------------- //

void BaseCharacter::Render(const unsigned int frameCount)
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
	if(mFrameProgression > mFramesPerAnimation)
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

	S2D::Vector2 mRenderPosition;

	if (mIsAlive)
	{
		// First calculate the source rect
		mSourceRect.X = float((mCurrentFrame % mSpritesOnWidthMain) * mSingleSpriteWidthMain);
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
		mSourceRect.X = float((mCurrentFrame % mSpritesOnWidthAlternate) * mSingleSpriteWidthAlternate);
		mSourceRect.Y = float(int(mCurrentFrame / mSpritesOnWidthAlternate) * mSingleSpriteHeightAlternate);

		mRenderPosition = S2D::Vector2(mCentrePosition.X - ((mSingleSpriteWidthAlternate * 0.5f) / SPRITE_RESOLUTION), mCentrePosition.Y - ((mSingleSpriteHeightAlternate * 0.5f) / SPRITE_RESOLUTION)) * SPRITE_RESOLUTION;

		// Now render the character in the correct position, 0.0 being the top left of the screen
		S2D::SpriteBatch::Draw(mAlternateSpritesSheet,
			                 &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
			                 &mSourceRect); // Draws Pacman
	}
}

// -------------------------------------------------------------------------------- //

void BaseCharacter::Update(const float deltaTime)
{
	// Now check if the player has hit the edge of the playable area so loop
	EdgeCheck();
}

// -------------------------------------------------------------------------------- //

void BaseCharacter::HandleInput()
{
	// Reset the requested direction
	mRequestedFacingDirection = FACING_DIRECTION::NONE;

	// Gets the current state of the keyboard
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	// Horizontal movement first 
	if (mCurrentFacingDirection == FACING_DIRECTION::NONE || mCurrentFacingDirection == FACING_DIRECTION::RIGHT || mCurrentFacingDirection == FACING_DIRECTION::LEFT)
	{
		// Can swap to the other axis just fine
		if (keyboardState->IsKeyDown(S2D::Input::Keys::W) && mCurrentFacingDirection != FACING_DIRECTION::UP && CanMoveInDirection(FACING_DIRECTION::UP))
		{
			mRequestedFacingDirection = FACING_DIRECTION::UP;
			return;
		}
		else if (keyboardState->IsKeyDown(S2D::Input::Keys::S) && mCurrentFacingDirection != FACING_DIRECTION::DOWN && CanMoveInDirection(FACING_DIRECTION::DOWN))
		{
			mRequestedFacingDirection = FACING_DIRECTION::DOWN;
			return;
		}

		// Now check for the same axis
		if (keyboardState->IsKeyDown(S2D::Input::Keys::D) && !keyboardState->IsKeyDown(S2D::Input::Keys::A) && mCurrentFacingDirection != FACING_DIRECTION::RIGHT && CanMoveInDirection(FACING_DIRECTION::RIGHT))
		{
			mRequestedFacingDirection = FACING_DIRECTION::RIGHT;
			return;
		}
		else if (keyboardState->IsKeyDown(S2D::Input::Keys::A) && !keyboardState->IsKeyDown(S2D::Input::Keys::D) && mCurrentFacingDirection != FACING_DIRECTION::LEFT && CanMoveInDirection(FACING_DIRECTION::LEFT))
		{
			mRequestedFacingDirection = FACING_DIRECTION::LEFT;
			return;
		}
	}

	if (mCurrentFacingDirection == FACING_DIRECTION::NONE || mCurrentFacingDirection == FACING_DIRECTION::UP || mCurrentFacingDirection == FACING_DIRECTION::DOWN)// Horizontal movement
	{
		// Can swap to the other axis just fine
		if (keyboardState->IsKeyDown(S2D::Input::Keys::A) && mCurrentFacingDirection != FACING_DIRECTION::LEFT && CanMoveInDirection(FACING_DIRECTION::LEFT))
		{
			mRequestedFacingDirection = FACING_DIRECTION::LEFT;
			return;
		}
		else if (keyboardState->IsKeyDown(S2D::Input::Keys::D) && mCurrentFacingDirection != FACING_DIRECTION::RIGHT && CanMoveInDirection(FACING_DIRECTION::RIGHT))
		{
			mRequestedFacingDirection = FACING_DIRECTION::RIGHT;
			return;
		}

		// Now check for the same axis
		if (keyboardState->IsKeyDown(S2D::Input::Keys::W) && !keyboardState->IsKeyDown(S2D::Input::Keys::S) && mCurrentFacingDirection != FACING_DIRECTION::UP && CanMoveInDirection(FACING_DIRECTION::UP))
		{
			mRequestedFacingDirection = FACING_DIRECTION::UP;
			return;
		}
		else if (keyboardState->IsKeyDown(S2D::Input::Keys::S) && !keyboardState->IsKeyDown(S2D::Input::Keys::W) && mCurrentFacingDirection != FACING_DIRECTION::DOWN && CanMoveInDirection(FACING_DIRECTION::DOWN))
		{
			mRequestedFacingDirection = FACING_DIRECTION::DOWN;
			return;
		}
	}
}

// -------------------------------------------------------------------------------- //

void BaseCharacter::ResetCharacter()
{
	mCentrePosition           = mStartPosition;
	mCurrentFacingDirection   = FACING_DIRECTION::NONE;
	mRequestedFacingDirection = FACING_DIRECTION::NONE;

	mStartFrame   = 0;
	mEndFrame     = 0;
	mCurrentFrame = 0;
}

// -------------------------------------------------------------------------------- //

bool BaseCharacter::CanMoveInDirection(FACING_DIRECTION direction)
{
	S2D::Vector2 gridPos          = S2D::Vector2(0.0f, 0.0f);

	switch (direction)
	{
	case FACING_DIRECTION::DOWN:
		gridPos = S2D::Vector2(mCentrePosition.X, mCentrePosition.Y + (mSingleSpriteHeightMain * 0.5f) / SPRITE_RESOLUTION);
	break;

	case FACING_DIRECTION::UP:
		gridPos = S2D::Vector2(mCentrePosition.X, mCentrePosition.Y - (mSingleSpriteHeightMain * 0.5f) / SPRITE_RESOLUTION);
	break;

	case FACING_DIRECTION::LEFT:
		gridPos = S2D::Vector2(mCentrePosition.X - (mSingleSpriteWidthMain * 0.5f) / SPRITE_RESOLUTION, mCentrePosition.Y);
	break;

	case FACING_DIRECTION::RIGHT:
		gridPos = S2D::Vector2(mCentrePosition.X + (mSingleSpriteWidthMain * 0.5f) / SPRITE_RESOLUTION, mCentrePosition.Y);
	break;
	}

	// Check if it is a valid direction change
	if (mCollisionMap[(unsigned int)gridPos.Y][(unsigned int)gridPos.X] == '0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

// ----------------------------------------------------------------- //

bool BaseCharacter::EdgeCheck()
{
	// First check if the character is going to go off the screen
	if (mCentrePosition.X + 0.5f <= 0 && mCurrentFacingDirection == FACING_DIRECTION::LEFT)
	{
		mCentrePosition.X = unsigned int(SCREEN_WIDTH / SPRITE_RESOLUTION);
		mMoveToPos        = mCentrePosition;
		return true;
	}

	if (mCentrePosition.X - 0.5f >= unsigned int(SCREEN_WIDTH / SPRITE_RESOLUTION) && mCurrentFacingDirection == FACING_DIRECTION::RIGHT)
	{
		mCentrePosition.X = 0.0f;
		mMoveToPos        = mCentrePosition;
		return true;
	}

	return false;
}

// ------------------------------------------------------------- //

void BaseCharacter::MoveInCurrentDirection(const float deltaTime)
{
	S2D::Vector2 gridPos            = S2D::Vector2(0.0f, 0.0f), movementAmount = S2D::Vector2(0.0f, 0.0f);

	float characterMovementDistance = mMovementSpeed * deltaTime;

	// First lock the opposite axis to which we are moving in
	if (mCurrentFacingDirection == FACING_DIRECTION::DOWN || mCurrentFacingDirection == FACING_DIRECTION::UP)
	{
		// If going downwards then we need to lock the character's X position
		mCentrePosition.X = ((int)mCentrePosition.X + 0.5f);
	}
	else if (mCurrentFacingDirection == FACING_DIRECTION::LEFT || mCurrentFacingDirection == FACING_DIRECTION::RIGHT)
	{
		mCentrePosition.Y = ((int)mCentrePosition.Y + 0.5f);
	}

	// Now we need to move the character in the direction selected, if we can
	switch (mCurrentFacingDirection)
	{
	case FACING_DIRECTION::DOWN:
		gridPos          = S2D::Vector2(mCentrePosition.X, mCentrePosition.Y + (mSingleSpriteHeightMain * 0.4f) / SPRITE_RESOLUTION);
		movementAmount.Y = characterMovementDistance;
	break;

	case FACING_DIRECTION::UP:
		gridPos          = S2D::Vector2(mCentrePosition.X, mCentrePosition.Y - (mSingleSpriteHeightMain * 0.4f) / SPRITE_RESOLUTION);
		movementAmount.Y = -1.0f * characterMovementDistance;
	break;

	case FACING_DIRECTION::LEFT:
		gridPos          = S2D::Vector2(mCentrePosition.X - (mSingleSpriteWidthMain * 0.4f) / SPRITE_RESOLUTION, mCentrePosition.Y);
		movementAmount.X = -1.0f * characterMovementDistance;
	break;

	case FACING_DIRECTION::RIGHT:
		gridPos          = S2D::Vector2(mCentrePosition.X + (mSingleSpriteWidthMain * 0.4f) / SPRITE_RESOLUTION, mCentrePosition.Y);
		movementAmount.X = characterMovementDistance;
	break;

	case FACING_DIRECTION::NONE:
		gridPos        = mCentrePosition;
		movementAmount = S2D::Vector2(0.0f, 0.0f);
	break;

	default:
		std::cout << "In an error movement state!" << std::endl;
	return;
	}

	// Now check if we can actually go to the new position - factoring in what the current state the ghost is in

	// If the next position is not blocked AND it is one of these specific situations
	if (mCollisionMap[(unsigned int)gridPos.Y][(unsigned int)gridPos.X] != '1')
	{
		mCentrePosition.X += movementAmount.X;
		mCentrePosition.Y += movementAmount.Y;
	}
	else
	{
		if (movementAmount.X > 0)
			mCentrePosition.X = (int)mCentrePosition.X + 0.5f;
		else
			mCentrePosition.Y = (int)mCentrePosition.Y + 0.5f;

		mCurrentFacingDirection = FACING_DIRECTION::NONE;
	}
}

// ------------------------------------------------------------- //

bool BaseCharacter::CanTurnToDirection(const FACING_DIRECTION newDir)
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

	// Check if the position is open
	if (mCollisionMap[(unsigned int)(int(mCentrePosition.Y) + offset.Y)][(unsigned int)(int(mCentrePosition.X) + offset.X)] == '0')
	{
		return true;
	}

	return false;
}

// ------------------------------------------------------------- //

void BaseCharacter::CheckForDirectionChange()
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

		// Set the new direction of facing
		mCurrentFacingDirection = mRequestedFacingDirection;
	}
}

// ------------------------------------------------------------- //

void BaseCharacter::SetIsAlive(const bool newVal)
{
	mIsAlive = newVal; 

	if (mIsAlive) 
		mMovementSpeed = GHOST_MOVEMENT_SPEED; 
	else 
		mMovementSpeed = GHOST_EYE_MOVEMENT_SPEED;
}

// ------------------------------------------------------------- //

void BaseCharacter::SetToMoveInDirection(FACING_DIRECTION direction)
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

// ------------------------------------------------------------- //

void BaseCharacter::CalculateAIMovementDirection()
{
	// Now calculate where we need to go to get to the taget position
	S2D::Vector2 movementDifferential = mTargetPositon - mCentrePosition;

	// First calculate which movements are valid and then choose which one is the best for the current situation
	bool canMoveUp    = CanTurnToDirection(FACING_DIRECTION::UP) && mCurrentFacingDirection != FACING_DIRECTION::DOWN;
	bool canMoveDown  = CanTurnToDirection(FACING_DIRECTION::DOWN) && mCurrentFacingDirection != FACING_DIRECTION::UP;
	bool canMoveRight = CanTurnToDirection(FACING_DIRECTION::RIGHT) && mCurrentFacingDirection != FACING_DIRECTION::LEFT;
	bool canMoveLeft  = CanTurnToDirection(FACING_DIRECTION::LEFT) && mCurrentFacingDirection != FACING_DIRECTION::RIGHT;

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