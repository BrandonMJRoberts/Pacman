#include "Pickups.h"

#include "GameManager.h"

#include <iostream>

// -------------------------------------------------------- //

PickUps::PickUps()
{
	// Default values
	mCentrePosition = S2D::Vector2(14 * SPRITE_RESOLUTION, 23.5 * SPRITE_RESOLUTION);
	mThisPickupType = PICKUP_TYPES::CHERRY;

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

PickUps::PickUps(PICKUP_TYPES type, S2D::Vector2 spawnPosition)
{
	mThisPickupType = type;
	mCentrePosition = spawnPosition;

	LoadSpriteSheet();

	SetupRenderData();
}

// -------------------------------------------------------- //

void PickUps::Render()
{
	if (mSpriteSheet && mSourceRect)
	{
		S2D::Vector2 mRenderPosition = S2D::Vector2(mCentrePosition - S2D::Vector2(mSingleSpriteWidth / 2.0f, mSingleSpriteHeight / 2.0f));

		S2D::SpriteBatch::Draw(mSpriteSheet, new S2D::Vector2(GameManager::Instance()->GetGridOffset() + mRenderPosition), mSourceRect);
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

	mSourceRect = new S2D::Rect( (int)mThisPickupType * mSingleSpriteWidth, 0.0f, mSingleSpriteWidth, mSingleSpriteHeight);
}

// -------------------------------------------------------- //

bool PickUps::CheckForCollision(const S2D::Vector2 pacmanCentrePosition, const unsigned int pacmanDimensions)
{
	if (mCentrePosition.X - (mSingleSpriteWidth / 2) < pacmanCentrePosition.X + pacmanDimensions && mCentrePosition.X + (mSingleSpriteWidth / 2) > pacmanCentrePosition.X - pacmanDimensions)
	{
		if (mCentrePosition.Y + (mSingleSpriteHeight / 2) > pacmanCentrePosition.Y - pacmanDimensions && mCentrePosition.Y - (mSingleSpriteHeight / 2) < pacmanCentrePosition.Y + pacmanDimensions)
		{
			return true;
		}
	}

	return false;
}

// -------------------------------------------------------- //
