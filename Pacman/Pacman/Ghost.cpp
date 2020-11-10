#include "Ghost.h"
#include "Constants.h"

#include <iostream>

S2D::Texture2D* Ghost::mSpriteSheet = nullptr;
S2D::Rect       Ghost::mSourceRect  = S2D::Rect();
S2D::Rect       Ghost::mDestRect    = S2D::Rect();

unsigned int    Ghost::mSingleSpriteWidth  = 0;
unsigned int    Ghost::mSingleSpriteHeight = 0;
char**          Ghost::mCollisionMap = nullptr;

// -------------------------------------------------------------------- //

Ghost::Ghost(S2D::Vector2 startPos, char** collisionMap, GHOST_TYPE ghostType, char* FilePath, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight) : mSpritesOnWidth(spritesOnWidth), mSpritesOnHeight(spritesOnHeight)
{
	if(!mCollisionMap)
		mCollisionMap  = collisionMap;

	mPosition      = startPos;
	mThisGhostType = ghostType;

	switch (mThisGhostType)
	{
	case GHOST_TYPE::RED:
		mCurrentFrame = 0;
		mStartFrame   = 0;
		mEndFrame     = 0;
	break;

	case GHOST_TYPE::ORANGE:
		mCurrentFrame = 0;
		mStartFrame   = 0;
		mEndFrame     = 0;
	break;

	case GHOST_TYPE::PINK:
		mCurrentFrame = 0;
		mStartFrame   = 0;
		mEndFrame     = 0;
	break; 

	case GHOST_TYPE::BLUE:
		mCurrentFrame = 0;
		mStartFrame   = 0;
		mEndFrame     = 0;
	break;

	default:
	break;
	}

	if (!mSpriteSheet)
	{
		LoadInSpriteSheet(FilePath);
	}

	if (mSpriteSheet)
	{
		mSingleSpriteWidth  = mSpriteSheet->GetWidth() / mSpritesOnWidth;
		mSingleSpriteHeight = mSpriteSheet->GetHeight() / mSpritesOnHeight;

		mSourceRect.Width   = mSingleSpriteWidth;
		mSourceRect.Height  = mSingleSpriteHeight;

		mDestRect.Width     = mSingleSpriteWidth;
		mDestRect.Height    = mSingleSpriteHeight;
	}
	else
		return;

}

// -------------------------------------------------------------------- //

Ghost::~Ghost()
{
	delete mSpriteSheet;
	mSpriteSheet = nullptr;

	mCollisionMap = nullptr;
}

// -------------------------------------------------------------------- //

void Ghost::Render()
{
	// First calculate the source rect
	mSourceRect.X = float(   (mCurrentFrame % mSpritesOnWidth) * mSingleSpriteWidth);
	mSourceRect.Y = float(int(mCurrentFrame / mSpritesOnWidth) * mSingleSpriteHeight);

	// Set the dest rect now
	mDestRect.X   = mPosition.X * SPRITE_RESOLUTION;
	mDestRect.Y   = mPosition.Y * SPRITE_RESOLUTION;

	// Do the draw call
	S2D::SpriteBatch::Draw(mSpriteSheet, &mDestRect, &mSourceRect);
}

// -------------------------------------------------------------------- //

void Ghost::Update(const float deltaTime, S2D::Vector2 pacmanPos)
{
	// First handle input

	// Now handle changing of direction

	// Now move the ghost in the currently facing direction
}

// -------------------------------------------------------------------- //

S2D::Vector2 Ghost::CalculateTargetPosition(S2D::Vector2 pacmanGridPos, FACING_DIRECTION pacmanFacingDirection)
{
	switch (mThisGhostType)
	{
	case GHOST_TYPE::RED:

	break;

	case GHOST_TYPE::ORANGE:
	break;

	case GHOST_TYPE::PINK:
	break;

	case GHOST_TYPE::BLUE:
	break;

	default:
	break;
	}

	return S2D::Vector2();
}

// -------------------------------------------------------------------- //

void Ghost::LoadInSpriteSheet(char* filePath)
{
	mSpriteSheet = new S2D::Texture2D();
	mSpriteSheet->Load(filePath, false);

	if (!mSpriteSheet)
	{
		std::cout << "Failed to load in the ghost sprite sheet" << std::endl;
		return;
	}
}

// -------------------------------------------------------------------- //