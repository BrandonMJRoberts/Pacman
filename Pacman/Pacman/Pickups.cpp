#include "Pickups.h"

#include "GameManager.h"
#include "UIManager.h"
#include "AudioManager.h"

#include <iostream>

// -------------------------------------------------------- //

PickUps::PickUps()
{
	// Default values
	mCentrePosition   = S2D::Vector2(14 * SPRITE_RESOLUTION, 17.5 * SPRITE_RESOLUTION);
	mThisPickupType   = PICKUP_TYPES::CHERRY;

	mCollidedWith     = false;
	mCollidedFromLeft = false;
	mCollected        = false;

	LoadSpriteSheet();

	SetupRenderData();
}

// -------------------------------------------------------- //

PickUps::~PickUps()
{
	delete mSpriteSheet;
		mSpriteSheet = nullptr;

	delete mSourceRect;
		mSourceRect = nullptr;
}

// -------------------------------------------------------- //

PickUps::PickUps(const PICKUP_TYPES type, const S2D::Vector2 spawnPosition)
{
	mThisPickupType   = type;
	mCentrePosition   = spawnPosition;

	mCollidedWith     = false;
	mCollidedFromLeft = false;

	LoadSpriteSheet();

	SetupRenderData();
}

// -------------------------------------------------------- //

void PickUps::Render()
{
	if (mSpriteSheet && mSourceRect)
	{
		S2D::Vector2 mRenderPosition = S2D::Vector2(mCentrePosition - S2D::Vector2(mSingleSpriteWidth / 2.0f, mSingleSpriteHeight / 2.0f));

		S2D::SpriteBatch::Draw(mSpriteSheet, &(GameManager::Instance()->GetGridOffset() + mRenderPosition), mSourceRect);
	}
}

// -------------------------------------------------------- //

void PickUps::Render(const S2D::Vector2 positionToRender)
{
	if (mSpriteSheet && mSourceRect)
	{
		S2D::SpriteBatch::Draw(mSpriteSheet, &positionToRender, mSourceRect);
	}
}

// -------------------------------------------------------- //

void PickUps::LoadSpriteSheet()
{
	delete mSpriteSheet;
	mSpriteSheet = new S2D::Texture2D();

	mSpriteSheet->Load("Textures/Fruit/Pickups.png", false);

	if (!mSpriteSheet)
	{
		std::cout << "Failed to load in the sprite sheet for the fruit pickups." << std::endl;
	}
}

// -------------------------------------------------------- //

void PickUps::SetupRenderData()
{
	mSingleSpriteWidth  = mSpriteSheet->GetWidth() / 8;
	mSingleSpriteHeight = mSpriteSheet->GetHeight();

	mSourceRect         = new S2D::Rect( float((int)mThisPickupType * mSingleSpriteWidth), 0.0f, mSingleSpriteWidth, mSingleSpriteHeight);
}

// -------------------------------------------------------- //

bool PickUps::CheckForCollision(const S2D::Vector2 pacmanCentrePosition, const unsigned int pacmanDimensions, const FACING_DIRECTION pacmanFacingDirection)
{
	// First check if pacman has collided with the with this pickup
	if (!mCollidedWith)
	{
		// If the far left of this is to the left of pacman's far right and the far right is to the right of pacman's far left then the X is a collision
		if (mCentrePosition.X - (mSingleSpriteWidth / 2) < pacmanCentrePosition.X + pacmanDimensions && mCentrePosition.X + (mSingleSpriteWidth / 2) > pacmanCentrePosition.X - pacmanDimensions)
		{
			// If the bottom is below pacman's top and the top is above pacman's bottom then the Y is a collision
			if (mCentrePosition.Y + (mSingleSpriteHeight / 2) > pacmanCentrePosition.Y - pacmanDimensions && mCentrePosition.Y - (mSingleSpriteHeight / 2) < pacmanCentrePosition.Y + pacmanDimensions)
			{
				// Set that we have collided
				mCollidedWith = true;

				// Now we know that we have been collided with we need to calculate which direction we have been collided from
				if (pacmanFacingDirection == FACING_DIRECTION::RIGHT)
					mCollidedFromLeft = true;

				return false;
			}
		}
	}
	else
	{
		// So we have already collided, so now check that the player has gone over enough of the pickup to collect it
		if (mCollidedFromLeft)
		{
			// If going to the right then we need to check if the far left of pacman is beyond the far left of the collectable
			if (pacmanCentrePosition.X - pacmanDimensions > mCentrePosition.X - (mSingleSpriteWidth / 2))
			{
				mCollected = true;
			}
			else
				return false;
		}
		else
		{
			// If pacman's far right is to the left of the pickup's far right
			if (pacmanCentrePosition.X + pacmanDimensions < mCentrePosition.X + (mSingleSpriteHeight / 2))
			{
				mCollected = true;
			}
			else
				return false;
		}

		if (mCollected)
		{
			S2D::Vector2 renderPos = S2D::Vector2(mCentrePosition + GameManager::Instance()->GetGridOffset());
			unsigned int magnitude = 0;

			// Increase the score
			switch (mThisPickupType)
			{
			case PICKUP_TYPES::CHERRY:
				GameManager::Instance()->AddToScore(100);
				magnitude = 0;
			break;

			case PICKUP_TYPES::APPLE:
				GameManager::Instance()->AddToScore(700);
				magnitude = 3;
			break;

			case PICKUP_TYPES::BELL:
				GameManager::Instance()->AddToScore(3000);
				magnitude = 6;
			break;

			case PICKUP_TYPES::CUP:
				GameManager::Instance()->AddToScore(2000);
				magnitude = 5;
			break;

			case PICKUP_TYPES::KEY:
				GameManager::Instance()->AddToScore(5000);
				magnitude = 7;
			break;

			case PICKUP_TYPES::LIME:
				GameManager::Instance()->AddToScore(1000);
				magnitude = 4;
			break;

			case PICKUP_TYPES::PEACH:
				GameManager::Instance()->AddToScore(500);
				magnitude = 2;
			break;

			case PICKUP_TYPES::STRAWBERRY:
				GameManager::Instance()->AddToScore(300);
				magnitude = 1;
			break;
			}

			UIManager::GetInstance()->DisplayPoints(renderPos, false, magnitude);
			AudioManager::GetInstance()->PlayEatingFruitSFX();


			// We have been passed over so return this to the higher function call
			return true;
		}
	}

	return false;














	/*
	if (!mCollidedWith)
	{
		if (mCentrePosition.X - (mSingleSpriteWidth / 2) < pacmanCentrePosition.X + pacmanDimensions && mCentrePosition.X + (mSingleSpriteWidth / 2) > pacmanCentrePosition.X - pacmanDimensions)
		{
			if (mCentrePosition.Y + (mSingleSpriteHeight / 2) > pacmanCentrePosition.Y - pacmanDimensions && mCentrePosition.Y - (mSingleSpriteHeight / 2) < pacmanCentrePosition.Y + pacmanDimensions)
			{
				mCollidedWith = true;

				// Now we know that we have been collided with we need to calculate which direction we have been collided from
				if (pacmanFacingDirection == FACING_DIRECTION::RIGHT)
					mCollidedFromLeft = true;
			}
		}
	}
	else
	{
		// If the far right is to the left of their far left then we have been passed ove
		if ( (pacmanCentrePosition.X - pacmanDimensions > mCentrePosition.X - (mSingleSpriteWidth * 0.2f) && mCollidedFromLeft) ||
			 (pacmanCentrePosition.X + pacmanDimensions < mCentrePosition.X + (mSingleSpriteWidth * 0.2f) && !mCollidedFromLeft) )
		{
			// Increase the score
			switch (mThisPickupType)
			{
			case PICKUP_TYPES::CHERRY:
				GameManager::Instance()->AddToScore(100);
			break;

			case PICKUP_TYPES::APPLE:
				GameManager::Instance()->AddToScore(700);
			break;

			case PICKUP_TYPES::BELL:
				GameManager::Instance()->AddToScore(3000);
			break;

			case PICKUP_TYPES::CUP:
				GameManager::Instance()->AddToScore(2000);
			break;

			case PICKUP_TYPES::KEY:
				GameManager::Instance()->AddToScore(5000);
			break;

			case PICKUP_TYPES::LIME:
				GameManager::Instance()->AddToScore(1000);
			break;

			case PICKUP_TYPES::PEACH:
				GameManager::Instance()->AddToScore(500);
			break;

			case PICKUP_TYPES::STRAWBERRY:
				GameManager::Instance()->AddToScore(300);
			break;
			}

			// We have been passed over so return this to the higher function call
			return true;
		}
	}
	*/

	// Return that we dont want to destroy this pickup yet
	//return false;
}

// -------------------------------------------------------- //
