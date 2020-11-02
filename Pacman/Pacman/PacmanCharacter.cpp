#include "PacmanCharacter.h"

#include "Constants.h"
#include "GameManager.h"

#include <iostream>

// ------------------------------------------------------------- //

PacmanCharacter::PacmanCharacter(char** collisionMap, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight) : mSpritesOnWidth(spritesOnWidth), mSpritesOnHeight(spritesOnHeight)
{
	// Load in the texture
	mPacmanTexture                     = new S2D::Texture2D();
	mPacmanTexture->Load("Textures/Pacman/PacmanSprites.png", false);
	if (!mPacmanTexture)
	{
		std::cout << "Failed to load the pacman sprite." << std::endl;
		return;
	}

	// Set the width and height variables
	mSingleSpriteWidth                 = mPacmanTexture->GetWidth()  / spritesOnWidth;
	mSingleSpriteHeight                = mPacmanTexture->GetHeight() / spritesOnHeight;

	// Set the starting position
	mStartPosition                     = S2D::Vector2(14 * SPRITE_RESOLUTION, 23.5 * SPRITE_RESOLUTION);
	ResetCharacter();

	mCurrentFacingDirection            = FACING_DIRECTION::NONE;
	mRequestedFacingDirection          = FACING_DIRECTION::NONE;

	mChangeDirectionInputDelay         = PLAYER_CHANGE_DIRECTION_DELAY;

	// Setup the rendering rectangle
	mPacmanSourceRect                  = new S2D::Rect(0.0f, 0.0f, mSingleSpriteWidth, mSingleSpriteHeight);
	if (!mPacmanSourceRect)
	{
		std::cout << "Failed to setup the pacman render rectangle." << std::endl;
		return;
	}

	mCollisionMap = collisionMap;

	mCurrentFrame = 0;
	mStartFrame   = 0;
	mEndFrame     = 0;
}

// ------------------------------------------------------------- //

PacmanCharacter::~PacmanCharacter()
{
	delete mPacmanSourceRect;
		mPacmanSourceRect = nullptr;

	delete mPacmanTexture;
		mPacmanTexture = nullptr;

	mCollisionMap = nullptr;
}

// ------------------------------------------------------------- //

void PacmanCharacter::Render(unsigned int currentFrameCount)
{
	if ((currentFrameCount % 6) == 1)
		mCurrentFrame++;

	if (mCurrentFrame > mEndFrame)
		mCurrentFrame = mStartFrame;

	if (mPacmanSourceRect && mPacmanTexture)
	{
		// Calculate the render position
		mRenderPosition = S2D::Vector2(mCentrePosition - S2D::Vector2(mSingleSpriteWidth / 2.0f, mSingleSpriteHeight / 2.0f));

		if (mCurrentFrame == mEndFrame)
		{
			mPacmanSourceRect->X = (float)mSingleSpriteWidth * 2;
			mPacmanSourceRect->Y = (float)mSingleSpriteWidth * 2;
		}
		else
		{
			mPacmanSourceRect->X = (float)mSingleSpriteWidth    *    (mCurrentFrame % mSpritesOnWidth);
			mPacmanSourceRect->Y = (float)mSingleSpriteHeight   * int(mCurrentFrame / mSpritesOnWidth);
		}

		// Render pacman in the correct position referencing his top left position, as the grid has 0,0 at the top left of the screen
		S2D::SpriteBatch::Draw(mPacmanTexture,
							   &(GameManager::Instance()->GetGridOffset() + mRenderPosition),
							   mPacmanSourceRect); // Draws Pacman
	}
}

// ------------------------------------------------------------- //

bool PacmanCharacter::EdgeCheck()
{
	// First check if pacman is going to go off the screen
	if (mCentrePosition.X + (mSingleSpriteWidth / 2) + 1 <= 0 && mCurrentFacingDirection == FACING_DIRECTION::LEFT)
	{
		mCentrePosition.X = float(SCREEN_WIDTH + (mSingleSpriteWidth / 2));
		return true;
	}

	if (mCentrePosition.X - (mSingleSpriteWidth / 2) - 1 >= SCREEN_WIDTH && mCurrentFacingDirection == FACING_DIRECTION::RIGHT)
	{
		mCentrePosition.X = -1.0f * (mSingleSpriteWidth / 2.0f);
		return true;
	}

	return false;
}

// ------------------------------------------------------------- //

void PacmanCharacter::MoveInCurrentDirection(const float deltaTime)
{
	S2D::Vector2 centreGridPos = ConvertPositionToGridPosition(mCentrePosition), gridPos;

	// First lock the opposite axis to which we are moving in
	if (mCurrentFacingDirection == FACING_DIRECTION::DOWN || mCurrentFacingDirection == FACING_DIRECTION::UP)
	{
		// If going downwards then we need to lock the player's X position
		mCentrePosition.X = SPRITE_RESOLUTION * ((int)centreGridPos.X + 0.5f);
	}
	else if (mCurrentFacingDirection == FACING_DIRECTION::LEFT || mCurrentFacingDirection == FACING_DIRECTION::RIGHT)
	{
		mCentrePosition.Y = SPRITE_RESOLUTION * ((int)centreGridPos.Y + 0.5f);
	}

	// Now we need to move pacman in the direction selected, if we can
	switch (mCurrentFacingDirection)
	{

	case FACING_DIRECTION::DOWN:
		// Convert the projected position of pacman into a grid position - now with the projection in his direction
		gridPos = ConvertPositionToGridPosition(mCentrePosition + S2D::Vector2(0.0f, mSingleSpriteHeight * 0.4f));// mSingleSpriteHeight / 3.0f));

		// If we are free to move in this direction then move in the direction
		if (mCollisionMap[(unsigned int)gridPos.Y][(unsigned int)gridPos.X] != '1')
		{
			mCentrePosition.Y     += PACMAN_MOVEMENT_SPEED * deltaTime;
		}
		else
		{
			mCentrePosition.Y       = SPRITE_RESOLUTION * ((int)centreGridPos.Y + 0.5f);
			mCurrentFacingDirection = FACING_DIRECTION::NONE;
		}
	break;

	case FACING_DIRECTION::UP:
		// Convert the projected position of pacman into a grid position
		gridPos = ConvertPositionToGridPosition(mCentrePosition + S2D::Vector2(0.0f, -1.0f * mSingleSpriteHeight * 0.4f));// (mSingleSpriteHeight / 2.0f)));

		if (mCollisionMap[(unsigned int)gridPos.Y][(unsigned int)gridPos.X] != '1')
		{
			mCentrePosition.Y     -= PACMAN_MOVEMENT_SPEED * deltaTime;
		}
		else
		{
			mCentrePosition.Y       = SPRITE_RESOLUTION * ((int)centreGridPos.Y + 0.5f);
			mCurrentFacingDirection = FACING_DIRECTION::NONE;
		}
	break;

	case FACING_DIRECTION::LEFT:

		// Convert the projected position of pacman into a grid position
		gridPos = ConvertPositionToGridPosition(mCentrePosition + S2D::Vector2(-1.0f * mSingleSpriteWidth * 0.4f, 0.0f)); //(mSingleSpriteWidth / 2.0f), 0.0f));

		if (mCollisionMap[(unsigned int)gridPos.Y][(unsigned int)gridPos.X] != '1')
		{
			mCentrePosition.X     -= PACMAN_MOVEMENT_SPEED * deltaTime;
		}
		else
		{
			mCentrePosition.X       = SPRITE_RESOLUTION * ((int)centreGridPos.X + 0.5f);
			mCurrentFacingDirection = FACING_DIRECTION::NONE;
		}
	break;

	case FACING_DIRECTION::RIGHT:

		// Convert the projected position of pacman into a grid position
		gridPos = ConvertPositionToGridPosition(mCentrePosition + S2D::Vector2(mSingleSpriteWidth * 0.4f ,0.0f));//mSingleSpriteWidth / 2.0f, 0.0f));

		if (mCollisionMap[(unsigned int)gridPos.Y][(unsigned int)gridPos.X] != '1')
		{
			mCentrePosition.X     += PACMAN_MOVEMENT_SPEED * deltaTime;
		}
		else
		{
			mCentrePosition.X       = SPRITE_RESOLUTION * ((int)centreGridPos.X + 0.5f);
			mCurrentFacingDirection = FACING_DIRECTION::NONE;
		}
	break;

	case FACING_DIRECTION::NONE:
	break;

	default:
		std::cout << "In an error movement state!" << std::endl;
		return;
	break;
	}
}

// ------------------------------------------------------------- //

void PacmanCharacter::CheckForDirectionChange()
{
	// Now check to see if the player can change direction 
	if (mRequestedFacingDirection == FACING_DIRECTION::NONE || mChangeDirectionInputDelay > 0.0f)
	{
		// Quick out if the player has not entered anything
		return;
	}
	else
	{
		// If we have changed direction then reset the countdown for the next change of direction
		mChangeDirectionInputDelay = PLAYER_CHANGE_DIRECTION_DELAY;

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
		}

		// Set the new direction of facing
		mCurrentFacingDirection = mRequestedFacingDirection;
	}
}

// ------------------------------------------------------------- //

void PacmanCharacter::Update(const float deltaTime)
{
	if(mChangeDirectionInputDelay > 0.0f)
		mChangeDirectionInputDelay -= deltaTime;

	// Now check if the player has hit the edge of the playable area so loop
	EdgeCheck();

	// Now move the player in the correct direction
	MoveInCurrentDirection(deltaTime);

	// First check for input
	HandleInput();

	// Now we have the requested direction to face we need to check if it is a valid change, so:
	// Now check if the player should change facing direction
	CheckForDirectionChange();

}

// ------------------------------------------------------------- //

void PacmanCharacter::HandleInput()
{
	// Reset the requested direction
	mRequestedFacingDirection = FACING_DIRECTION::NONE;

	// Gets the current state of the keyboard
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	// Checks if the directional keys have been pressed and set the requested change in direction
	if (keyboardState->IsKeyDown(S2D::Input::Keys::D) && mCurrentFacingDirection != FACING_DIRECTION::RIGHT && CanMoveInDirection(FACING_DIRECTION::RIGHT))
	{
		mRequestedFacingDirection = FACING_DIRECTION::RIGHT;
		return;
	}

	if (keyboardState->IsKeyDown(S2D::Input::Keys::W) && mCurrentFacingDirection != FACING_DIRECTION::UP && CanMoveInDirection(FACING_DIRECTION::UP))
	{
		mRequestedFacingDirection = FACING_DIRECTION::UP;
		return;
	}

	if (keyboardState->IsKeyDown(S2D::Input::Keys::S) && mCurrentFacingDirection != FACING_DIRECTION::DOWN && CanMoveInDirection(FACING_DIRECTION::DOWN))
	{
		mRequestedFacingDirection = FACING_DIRECTION::DOWN;
		return;
	}

	if (keyboardState->IsKeyDown(S2D::Input::Keys::A) && mCurrentFacingDirection != FACING_DIRECTION::LEFT && CanMoveInDirection(FACING_DIRECTION::LEFT))
	{
		mRequestedFacingDirection = FACING_DIRECTION::LEFT;
		return;
	}
}

// ------------------------------------------------------------- //

S2D::Vector2 PacmanCharacter::ConvertPositionToGridPosition(S2D::Vector2 position)
{
	return S2D::Vector2(position.X / SPRITE_RESOLUTION, position.Y / SPRITE_RESOLUTION);
}

// ------------------------------------------------------------- //

void PacmanCharacter::ReSetupPacmanSourceRect(float x, float y, unsigned int width, unsigned int height)
{
	delete mPacmanSourceRect;
	mPacmanSourceRect = new S2D::Rect(x, y, width, height);
}

// ------------------------------------------------------------- //

void PacmanCharacter::ResetCharacter()
{
	mCentrePosition           = mStartPosition; 
	mCurrentFacingDirection   = FACING_DIRECTION::RIGHT; 
	mRequestedFacingDirection = FACING_DIRECTION::RIGHT;

	ReSetupPacmanSourceRect(0, 0, mSingleSpriteWidth, mSingleSpriteHeight);
}

// ----------------------------------------------------------------- //

bool PacmanCharacter::CanMoveInDirection(FACING_DIRECTION direction)
{
	S2D::Vector2 gridPos = S2D::Vector2();

	switch (direction)
	{
	case FACING_DIRECTION::DOWN:

		// Calculate the correctly projected direction
		gridPos = ConvertPositionToGridPosition(mCentrePosition + S2D::Vector2(0.0f, mSingleSpriteHeight * 0.5f));

	break;

	case FACING_DIRECTION::UP:

		// Calculate the correctly projected direction
		gridPos = ConvertPositionToGridPosition(mCentrePosition + S2D::Vector2(0.0f, -1.0f * mSingleSpriteHeight * 0.5f));// (mSingleSpriteHeight / 2.0f)));

	break;

	case FACING_DIRECTION::LEFT:

		// Calculate the correctly projected direction
		gridPos = ConvertPositionToGridPosition(mCentrePosition + S2D::Vector2(-1.0f * (mSingleSpriteWidth * 0.5f), 0.0f));// (mSingleSpriteWidth / 2.0f), 0.0f));

	break;

	case FACING_DIRECTION::RIGHT:

		// Calculate the correctly projected direction
		gridPos = ConvertPositionToGridPosition(mCentrePosition + S2D::Vector2(mSingleSpriteWidth * 0.5f, 0.0f));// mSingleSpriteWidth / 2.0f, 0.0f));

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