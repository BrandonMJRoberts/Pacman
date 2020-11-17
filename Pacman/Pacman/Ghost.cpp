#include "Ghost.h"
#include "Constants.h"
#include "Commons.h"

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
		mCollisionMap         = collisionMap;

	mPosition                 = startPos;
	mThisGhostType            = ghostType;
	mIsPlayerControlled       = false;
	mCurrentFacingDirection   = FACING_DIRECTION::NONE;
	mRequestedFacingDirection = FACING_DIRECTION::NONE;

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
	if (mIsPlayerControlled)
	{

	}
	else
	{
		// Otherwise have the AI make the decision.

	}

	// Now handle changing of direction


	// Now move the ghost in the currently facing direction
}

// -------------------------------------------------------------------- //

S2D::Vector2 Ghost::ConvertPositionToGridPosition(S2D::Vector2 position)
{
	return S2D::Vector2(position.X / SPRITE_RESOLUTION, position.Y / SPRITE_RESOLUTION);
}

// -------------------------------------------------------------------- //

void Ghost::MoveInCurrentDirection(const float deltaTime)
{
	// Calculate the current grid postion of pacman's centre
	S2D::Vector2 centreGridPos = ConvertPositionToGridPosition(mPosition);
	S2D::Vector2 gridPos = S2D::Vector2(), movementAmount = S2D::Vector2();

	float ghostMovementDistance = GHOST_MOVEMENT_SPEED * deltaTime;

	// First lock the opposite axis to which we are moving in
	if (mCurrentFacingDirection == FACING_DIRECTION::DOWN || mCurrentFacingDirection == FACING_DIRECTION::UP)
	{
		// If going downwards then we need to lock the player's X position
		mPosition.X = SPRITE_RESOLUTION * ((int)centreGridPos.X + 0.5f);
	}
	else if (mCurrentFacingDirection == FACING_DIRECTION::LEFT || mCurrentFacingDirection == FACING_DIRECTION::RIGHT)
	{
		mPosition.Y = SPRITE_RESOLUTION * ((int)centreGridPos.Y + 0.5f);
	}

	// Now we need to move pacman in the direction selected, if we can
	switch (mCurrentFacingDirection)
	{

	case FACING_DIRECTION::DOWN:
		// Convert the projected position of pacman into a grid position - now with the projection in his direction
		gridPos = ConvertPositionToGridPosition(mPosition + S2D::Vector2(0.0f, mSingleSpriteHeight * 0.4f));// mSingleSpriteHeight / 3.0f));

		movementAmount.Y = ghostMovementDistance;
		break;

	case FACING_DIRECTION::UP:
		// Convert the projected position of pacman into a grid position
		gridPos = ConvertPositionToGridPosition(mPosition + S2D::Vector2(0.0f, -1.0f * mSingleSpriteHeight * 0.4f));// (mSingleSpriteHeight / 2.0f)));

		movementAmount.Y = -1.0f * ghostMovementDistance;
		break;

	case FACING_DIRECTION::LEFT:

		// Convert the projected position of pacman into a grid position
		gridPos = ConvertPositionToGridPosition(mPosition + S2D::Vector2(-1.0f * mSingleSpriteWidth * 0.4f, 0.0f)); //(mSingleSpriteWidth / 2.0f), 0.0f));

		movementAmount.X = -1.0f * ghostMovementDistance;
		break;

	case FACING_DIRECTION::RIGHT:

		// Convert the projected position of pacman into a grid position
		gridPos = ConvertPositionToGridPosition(mPosition + S2D::Vector2(mSingleSpriteWidth * 0.4f, 0.0f));

		movementAmount.X = ghostMovementDistance;
		break;

	case FACING_DIRECTION::NONE:
		break;

	default:
		std::cout << "In an error movement state!" << std::endl;
		return;
		break;
	}

	if (mCollisionMap[(unsigned int)gridPos.Y][(unsigned int)gridPos.X] != '1')
	{
		mPosition.X += movementAmount.X;
		mPosition.Y += movementAmount.Y;
	}
	else
	{
		if (movementAmount.X > 0)
			mPosition.X = SPRITE_RESOLUTION * ((int)centreGridPos.X + 0.5f);
		else
			mPosition.Y = SPRITE_RESOLUTION * ((int)centreGridPos.Y + 0.5f);

		mCurrentFacingDirection = FACING_DIRECTION::NONE;
	}
}

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