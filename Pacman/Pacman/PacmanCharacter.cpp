#include "PacmanCharacter.h"

#include "Constants.h"
#include "GameManager.h"

#include <iostream>

// ------------------------------------------------------------- //

PacmanCharacter::PacmanCharacter(char** collisionMap, unsigned int collisionMapWidth, unsigned int collisionMapHeight)
{
	mCurrentFacingDirection   = FACING_DIRECTION::NONE;
	mRequestedFacingDirection = FACING_DIRECTION::NONE;

	mPacmanTexture = new S2D::Texture2D();
	mPacmanTexture->Load("Textures/Pacman/PacmanAnimations.png", false);
	if (!mPacmanTexture)
	{
		std::cout << "Failed to load the pacman sprite." << std::endl;
		return;
	}

	mSpriteWidth  = mPacmanTexture->GetWidth();
	mSpriteHeight = mPacmanTexture->GetHeight();

	mSingleSpriteWidth  = mSpriteWidth  / 3;
	mSingleSpriteHeight = mSpriteHeight / 4;

	// Setup the rendering rectangle
	mPacmanSourceRect = new S2D::Rect(0.0f, 0.0f, mSingleSpriteWidth, mSingleSpriteHeight);
	if (!mPacmanSourceRect)
	{
		std::cout << "Failed to setup the pacman render rectangle." << std::endl;
		return;
	}

	// Default pacman's position to being the top left of the grid
	mPosition				 = S2D::Vector2(3.0f, 1.0f);

	if (collisionMap)
	{
		mCollisionMap    = collisionMap;

		mCollisionWidth  = collisionMapWidth;
		mCollisionHeight = collisionMapHeight;
	}
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
	unsigned int progressThroughAnimation = currentFrameCount % 12;

	if (progressThroughAnimation <= 3)
	{
		mPacmanSourceRect->X = 0;
	}
	else if (progressThroughAnimation <= 7)
	{
		mPacmanSourceRect->X = (float)mSingleSpriteWidth;
	}
	else
	{
		mPacmanSourceRect->X = (float)mSingleSpriteWidth * 2;
	}

	if(mPacmanSourceRect && mPacmanTexture)
		S2D::SpriteBatch::Draw(mPacmanTexture, new S2D::Vector2(GameManager::Instance()->GetGridOffset().X + (mPosition.X * SPRITE_RESOLUTION), 
			                                                    GameManager::Instance()->GetGridOffset().Y + (mPosition.Y * SPRITE_RESOLUTION)), 
			                                                    mPacmanSourceRect); // Draws Pacman
}

// ------------------------------------------------------------- //

bool PacmanCharacter::EdgeCheck()
{
	// First check if pacman is going to go off the screen
	if (mPosition.X <= -1 && mCurrentFacingDirection == FACING_DIRECTION::LEFT)
	{
		mPosition.X = (float)mCollisionWidth;
		return true;
	}

	if (mPosition.X >= mCollisionWidth && mCurrentFacingDirection == FACING_DIRECTION::RIGHT)
	{
		mPosition.X = -1;
		return true;
	}

	return false;
}

// ------------------------------------------------------------- //

void PacmanCharacter::MoveInCurrentDirection(const float deltaTime)
{
	// Now we need to move pacman in the direction selected, if we can
	switch (mCurrentFacingDirection)
	{
	case FACING_DIRECTION::DOWN:
		if (mCollisionMap[(unsigned int)mPosition.Y + 1][(unsigned int)mPosition.X] == '0')
		{
			mPosition.Y += PACMAN_MOVEMENT_SPEED * deltaTime;
		}
		break;

	case FACING_DIRECTION::UP:
		if (mCollisionMap[(unsigned int)mPosition.Y - 1][(unsigned int)mPosition.X] == '0')
		{
			mPosition.Y -= PACMAN_MOVEMENT_SPEED * deltaTime;
		}
		break;

	case FACING_DIRECTION::LEFT:
		if (mPosition.X > 1)
		{
			if (mCollisionMap[(unsigned int)mPosition.Y][(unsigned int)mPosition.X - 1] == '0')
			{
				mPosition.X -= PACMAN_MOVEMENT_SPEED * deltaTime;
			}
		}
		else
		{
			mPosition.X -= PACMAN_MOVEMENT_SPEED * deltaTime;
		}
	break;

	case FACING_DIRECTION::RIGHT:
		if (mPosition.X < mCollisionWidth - 1)
		{
			if (mCollisionMap[(unsigned int)mPosition.Y][(unsigned int)mPosition.X + 1] == '0')
			{
				mPosition.X += PACMAN_MOVEMENT_SPEED * deltaTime;
				return;
			}
		}
		else
		{
			mPosition.X += PACMAN_MOVEMENT_SPEED * deltaTime;
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
	if (mRequestedFacingDirection == FACING_DIRECTION::NONE)
	{
		return;
	}
	else
	{
		//unsigned int DoubleSpriteRes = SPRITE_RESOLUTION * 2;

		switch (mRequestedFacingDirection)
		{
		case FACING_DIRECTION::DOWN:
			if (mPosition.Y < mCollisionHeight - 1)
			{
				if (mCollisionMap[(unsigned int)mPosition.Y + 1][(unsigned int)mPosition.X] == '0')
				{
					mCurrentFacingDirection = mRequestedFacingDirection;

					// And setup the new source rect
					delete mPacmanSourceRect;
					mPacmanSourceRect = new S2D::Rect(0, (float)mSingleSpriteHeight * 3, mSingleSpriteWidth, mSingleSpriteHeight);
				}
			}
		break;

		case FACING_DIRECTION::UP:
			if (mPosition.Y > 0)
			{
				if (mCollisionMap[(unsigned int)mPosition.Y - 1][(unsigned int)mPosition.X] == '0')
				{
					mCurrentFacingDirection = mRequestedFacingDirection;

					delete mPacmanSourceRect;
					mPacmanSourceRect = new S2D::Rect(0, (float)mSingleSpriteHeight * 2, mSingleSpriteWidth, mSingleSpriteHeight);
				}
			}
		break;

		case FACING_DIRECTION::LEFT:
			if (mPosition.X >= 1)
			{
				if (mCollisionMap[(unsigned int)mPosition.Y][(unsigned int)mPosition.X - 1] == '0')
				{
					mCurrentFacingDirection = mRequestedFacingDirection;

					delete mPacmanSourceRect;
					mPacmanSourceRect = new S2D::Rect(0, (float)mSingleSpriteHeight, mSingleSpriteWidth, mSingleSpriteHeight);
				}
			}
		break;

		case FACING_DIRECTION::RIGHT:
			if (mPosition.X < mCollisionWidth - 1)
			{
				if (mCollisionMap[(unsigned int)mPosition.Y][(unsigned int)mPosition.X + 1] == '0')
				{
					mCurrentFacingDirection = mRequestedFacingDirection;

					delete mPacmanSourceRect;
					mPacmanSourceRect = new S2D::Rect(0, 0, mSingleSpriteWidth, mSingleSpriteHeight);
				}
			}
		break;

		default:
		break;
		}
	}
}

// ------------------------------------------------------------- //

void PacmanCharacter::Update(const float deltaTime)
{
	// First handle the player's input
	HandleInput(deltaTime);

	// First check if pacman should loop around the screen - if it should then 
	if (EdgeCheck())
	{
		return;
	}

	// Dont run any movement code if you dont have a collision map to check against
	if (mCollisionMap)
	{
		MoveInCurrentDirection(deltaTime);

		CheckForDirectionChange();
	}

	
}

// ------------------------------------------------------------- //

void PacmanCharacter::HandleInput(const float deltaTime)
{
	// Reset the requested direction
	mRequestedFacingDirection = FACING_DIRECTION::NONE;

	// Gets the current state of the keyboard
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	// Checks if the directional keys have been pressed and set the requested change in direction
	if (keyboardState->IsKeyDown(S2D::Input::Keys::D) && mCurrentFacingDirection != FACING_DIRECTION::RIGHT)
	{
		mRequestedFacingDirection = FACING_DIRECTION::RIGHT;
	}
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::A) && mCurrentFacingDirection != FACING_DIRECTION::LEFT)
	{
		mRequestedFacingDirection = FACING_DIRECTION::LEFT;
	}
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::W) && mCurrentFacingDirection != FACING_DIRECTION::UP)
	{
		mRequestedFacingDirection = FACING_DIRECTION::UP;
	}
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::S) && mCurrentFacingDirection != FACING_DIRECTION::DOWN)
	{
		mRequestedFacingDirection = FACING_DIRECTION::DOWN;
	}
}

// ------------------------------------------------------------- //