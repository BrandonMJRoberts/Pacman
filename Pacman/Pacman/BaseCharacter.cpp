#include "BaseCharacter.h"

#include "Constants.h"
#include "GameManager.h"

#include <iostream>

// -------------------------------------------------------------------------------- //

BaseCharacter::BaseCharacter(const char** const collisionMap, 
	                         const S2D::Vector2 startPosition, 
	                         const bool         isAIControlled, 
	                         const char*        filePathForMainSpriteSheet, 
	                         const char*        filePathForAlternateSpriteSheet, 
	                         const unsigned int spritesOnWidth, 
	                         const unsigned int spritesOnHeight) : mSpritesOnHeight(spritesOnHeight),
	                                                               mSpritesOnWidth(spritesOnWidth), 
	                                                               mCollisionMap(collisionMap), 
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
	mSingleSpriteHeight              = mMainSpriteSheet->GetHeight() / spritesOnHeight;
	mSingleSpriteWidth               = mMainSpriteSheet->GetWidth() / spritesOnWidth;

	// Set if the character is AI controlled
	mIsPlayerControlled              = !isAIControlled;

	// Store the start position
	mStartPosition                   = startPosition;
	mCentrePosition                  = startPosition;

	// Set the default values
	mTimePerChangeDirectionRemaining = mTimePerChangeOfDirection;
	mUsingMainSpriteSheet            = true;
	mMovementSpeed                   = 0.0f;

	mSourceRect.Width                = mSingleSpriteWidth;
	mSourceRect.Height               = mSingleSpriteHeight;

	mCurrentFacingDirection          = FACING_DIRECTION::NONE;
	mRequestedFacingDirection        = FACING_DIRECTION::NONE;

	mFramesPerAnimation              = 0;

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
	mSourceRect.X = float(   (mCurrentFrame % mSpritesOnWidth) * mSingleSpriteWidth);
	mSourceRect.Y = float(int(mCurrentFrame / mSpritesOnWidth) * mSingleSpriteHeight);

	// Now calculate the render position
	S2D::Vector2 mRenderPosition = S2D::Vector2(mCentrePosition.X - ((mSingleSpriteWidth * 0.5f) / SPRITE_RESOLUTION), mCentrePosition.Y - ((mSingleSpriteHeight * 0.5f) / SPRITE_RESOLUTION)) * SPRITE_RESOLUTION;

	if (mIsAlive && mMainSpriteSheet)
	{
		// Now render the character in the correct position, 0.0 being the top left of the screen
		S2D::SpriteBatch::Draw(mMainSpriteSheet,
							 &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
							 &mSourceRect); // Draws Pacman
	}
	else if(!mIsAlive && mAlternateSpritesSheet)
	{
		// Now render the character in the correct position, 0.0 being the top left of the screen
		S2D::SpriteBatch::Draw(mAlternateSpritesSheet,
			                 &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
			                 &mSourceRect); // Draws Pacman
	}
}

// -------------------------------------------------------------------------------- //

void BaseCharacter::Update(const float deltaTime)
{
	if (mTimePerChangeDirectionRemaining > 0.0f)
		mTimePerChangeDirectionRemaining -= deltaTime;

	// Now check if the player has hit the edge of the playable area so loop
	EdgeCheck();

	// Now move the player in the correct direction
	MoveInCurrentDirection(deltaTime);

	CheckForDirectionChange();
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
		gridPos = S2D::Vector2(mCentrePosition.X, mCentrePosition.Y + (mSingleSpriteHeight * 0.5f) / SPRITE_RESOLUTION);
	break;

	case FACING_DIRECTION::UP:
		gridPos = S2D::Vector2(mCentrePosition.X, mCentrePosition.Y - (mSingleSpriteHeight * 0.5f) / SPRITE_RESOLUTION);
	break;

	case FACING_DIRECTION::LEFT:
		gridPos = S2D::Vector2(mCentrePosition.X - (mSingleSpriteWidth * 0.5f) / SPRITE_RESOLUTION, mCentrePosition.Y);
	break;

	case FACING_DIRECTION::RIGHT:
		gridPos = S2D::Vector2(mCentrePosition.X + (mSingleSpriteWidth * 0.5f) / SPRITE_RESOLUTION, mCentrePosition.Y);
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
		return true;
	}

	if (mCentrePosition.X - 0.5f >= unsigned int(SCREEN_WIDTH / SPRITE_RESOLUTION) && mCurrentFacingDirection == FACING_DIRECTION::RIGHT)
	{
		mCentrePosition.X = 0;
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
		gridPos          = S2D::Vector2(mCentrePosition.X, mCentrePosition.Y + (mSingleSpriteHeight * 0.4f) / SPRITE_RESOLUTION);
		movementAmount.Y = characterMovementDistance;
	break;

	case FACING_DIRECTION::UP:
		gridPos          = S2D::Vector2(mCentrePosition.X, mCentrePosition.Y - (mSingleSpriteHeight * 0.4f) / SPRITE_RESOLUTION);
		movementAmount.Y = -1.0f * characterMovementDistance;
	break;

	case FACING_DIRECTION::LEFT:
		gridPos          = S2D::Vector2(mCentrePosition.X - (mSingleSpriteWidth * 0.4f) / SPRITE_RESOLUTION, mCentrePosition.Y);
		movementAmount.X = -1.0f * characterMovementDistance;
	break;

	case FACING_DIRECTION::RIGHT:
		gridPos          = S2D::Vector2(mCentrePosition.X + (mSingleSpriteWidth * 0.4f) / SPRITE_RESOLUTION, mCentrePosition.Y);
		movementAmount.X = characterMovementDistance;
	break;

	case FACING_DIRECTION::NONE:
	break;

	default:
		std::cout << "In an error movement state!" << std::endl;
	return;
	}

	// Now check if we can actually go to the new position
	if (mCollisionMap[(unsigned int)gridPos.Y][(unsigned int)gridPos.X] != '1') //gridPos.X > 0.0f && gridPos.X < SCREEN_WIDTH / SPRITE_RESOLUTION && gridPos.Y > 0.0f && gridPos.Y < SCREEN_HEIGHT / SPRITE_RESOLUTION && 
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
	S2D::Vector2 offset = S2D::Vector2();

	switch (newDir)
	{
	case FACING_DIRECTION::UP:
		offset.Y = -1;
	break;

	case FACING_DIRECTION::DOWN:
		offset.Y = 1;
	break;

	case FACING_DIRECTION::LEFT:
		offset.X = -1;
	break;

	case FACING_DIRECTION::RIGHT:
		offset.X = 1;
	break;
	}

	// Need to go right to get to the position
	if (mCollisionMap[(unsigned int)(mCentrePosition.Y + offset.Y)][(unsigned int)(mCentrePosition.X + offset.X)] == '0')
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