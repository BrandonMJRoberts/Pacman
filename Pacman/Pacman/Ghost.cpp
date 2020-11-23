#include "Ghost.h"
#include "Constants.h"
#include "Commons.h"

#include "GameManager.h"

#include <iostream>

S2D::Texture2D* Ghost::mColouredSpriteSheet = nullptr;
S2D::Texture2D* Ghost::mFleeSpriteSheet = nullptr;

S2D::Rect       Ghost::mSourceRect  = S2D::Rect();

unsigned int    Ghost::mSingleSpriteWidth  = 0;
unsigned int    Ghost::mSingleSpriteHeight = 0;
char**          Ghost::mCollisionMap = nullptr;

// -------------------------------------------------------------------- //

Ghost::Ghost(const S2D::Vector2 startPos, char** collisionMap, const GHOST_TYPE ghostType, const bool isAIControlled, const char* FilePathForColoured, const char* FilePathForFlee, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight) : mSpritesOnWidth(spritesOnWidth), mSpritesOnHeight(spritesOnHeight), mTimePerChangeOfDirection(GHOST_CHANGE_DIRECTION_DELAY)
{
	// Store the collsiion map
	if(!mCollisionMap)
		mCollisionMap         = collisionMap;

	// Set the default values
	mPosition                 = startPos;
	mThisGhostType            = ghostType;
	mIsPlayerControlled       = !isAIControlled;
	mCurrentFacingDirection   = FACING_DIRECTION::NONE;
	mRequestedFacingDirection = FACING_DIRECTION::NONE;

	mIsGhostAlive             = true;
	mGhostIsFleeing           = false;

	// Create the state machine needed
	mStateMachine             = new Stack_FiniteStateMachine_Ghost(ghostType, isAIControlled);

	mTimePerChangeDirectionRemaining = mTimePerChangeOfDirection;

	// Set the starting, ending and current frames
	switch (mThisGhostType)
	{
	case GHOST_TYPE::RED:
		mCurrentFrame = 0;
		mStartFrame   = 0;
		mEndFrame     = 1;
	break;

	case GHOST_TYPE::ORANGE:
		mCurrentFrame = 24;
		mStartFrame   = 24;
		mEndFrame     = 25;
	break;

	case GHOST_TYPE::PINK:
		mCurrentFrame = 8;
		mStartFrame   = 8;
		mEndFrame     = 9;
	break; 

	case GHOST_TYPE::BLUE:
		mCurrentFrame = 16;
		mStartFrame   = 16;
		mEndFrame     = 17;
	break;

	default:
	break;
	}

	// Load in the sprite sheets
	if (!mColouredSpriteSheet || !mFleeSpriteSheet)
	{
		LoadInSpriteSheets(FilePathForColoured, FilePathForFlee);
	}

	// Setup the render data
	if (mColouredSpriteSheet)
	{
		mSingleSpriteWidth  = mColouredSpriteSheet->GetWidth() / mSpritesOnWidth;
		mSingleSpriteHeight = mColouredSpriteSheet->GetHeight() / mSpritesOnHeight;

		mSourceRect.Width   = mSingleSpriteWidth;
		mSourceRect.Height  = mSingleSpriteHeight;
	}
	else
		return;

}

// -------------------------------------------------------------------- //

Ghost::~Ghost()
{
	// Clear up the memory used
	delete mColouredSpriteSheet;
	mColouredSpriteSheet = nullptr;

	delete mFleeSpriteSheet;
	mFleeSpriteSheet = nullptr;

	mCollisionMap = nullptr;
}

// -------------------------------------------------------------------- //

void Ghost::Render(const unsigned int currentFrameCount)
{
	// Check if we need to change what frame we are rendering
	if ((currentFrameCount % 12) == 1)
		mCurrentFrame++;

	// Make sure we loop if needed
	if (mCurrentFrame > mEndFrame)
		mCurrentFrame = mStartFrame;

	// Calculate the render position for the ghost
	S2D::Vector2 mRenderPosition = S2D::Vector2((mPosition * SPRITE_RESOLUTION) - S2D::Vector2(mSingleSpriteWidth / 2.0f, mSingleSpriteHeight / 2.0f));

	// First calculate the source rect
	mSourceRect.X = float(   (mCurrentFrame % mSpritesOnWidth) * mSingleSpriteWidth);
	mSourceRect.Y = float(int(mCurrentFrame / mSpritesOnWidth) * mSingleSpriteHeight);

	// Do the draw call
	if (mIsGhostAlive && !mGhostIsFleeing)
	{
		S2D::SpriteBatch::Draw(mColouredSpriteSheet, &(GameManager::Instance()->GetGridOffset() + mRenderPosition), &mSourceRect);
	}
	else if (!mIsGhostAlive || mGhostIsFleeing)
	{
		S2D::SpriteBatch::Draw(mFleeSpriteSheet, &(GameManager::Instance()->GetGridOffset() + mRenderPosition), &mSourceRect);
	}

}

// -------------------------------------------------------------------- //

void Ghost::Update(const float deltaTime, const S2D::Vector2 pacmanPos, const FACING_DIRECTION pacmanFacingDirection)
{
	// First the ghost's movemt 
	if (mIsPlayerControlled)
	{
		// Handle the player's input to determine which direction the ghost should go

	}
	else
	{
		// Otherwise have the AI make the decision.
		BaseState_Ghost* currentState = mStateMachine->PeekStack();
		if (currentState)
		{
			if (mThisGhostType != GHOST_TYPE::ORANGE)
			{
				// Update the current state  
				currentState->OnUpdate(mTargetPositon, pacmanPos, pacmanFacingDirection);
			}
			else if (mThisGhostType == GHOST_TYPE::ORANGE && mPosition == mTargetPositon)
			{
				currentState->OnUpdate(mTargetPositon, pacmanPos, pacmanFacingDirection);
			}

			// Check if we should transition out of the current state
			currentState->CheckTransitions(this);
		}

		mTimePerChangeDirectionRemaining -= deltaTime;

		if (mTimePerChangeDirectionRemaining <= 0.0f)
		{
			// Now calculate how we are going to get to where we want to go
			CalculateAIMovementDirection();

			mTimePerChangeDirectionRemaining = mTimePerChangeOfDirection;
		}
	}

	// Now move the ghost in the currently facing direction
	MoveInCurrentDirection(deltaTime);
}

// -------------------------------------------------------------------- //

bool Ghost::CanTurnToDirection(const FACING_DIRECTION newDir)
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
	if (mCollisionMap[(unsigned int)(mPosition.Y + offset.Y)][(unsigned int)(mPosition.X + offset.X)] == '0')
	{
		return true;
	}
	
	return false;
}

// -------------------------------------------------------------------- //

void Ghost::CalculateAIMovementDirection()
{
	// Now calculate where we need to go to get to the taget position
	S2D::Vector2 movementDifferential = mTargetPositon - mPosition;

	float checkingAccuracy            = 0.5f;

	// Check if we have arrived at the target position - if so then stop moveing
	if (movementDifferential.X <= checkingAccuracy && movementDifferential.X > -checkingAccuracy && movementDifferential.Y <= checkingAccuracy && movementDifferential.Y > -checkingAccuracy)
	{
		mCurrentFacingDirection = FACING_DIRECTION::NONE;
		return;
	}

	// First calculate which movements are valid and then choose which one is the best for the current situation
	bool canMoveUp    = CanTurnToDirection(FACING_DIRECTION::UP);
	bool canMoveDown  = CanTurnToDirection(FACING_DIRECTION::DOWN);
	bool canMoveRight = CanTurnToDirection(FACING_DIRECTION::RIGHT);
	bool canMoveLeft  = CanTurnToDirection(FACING_DIRECTION::LEFT);

	bool validVerticalMove   = false;
	bool validHorizontalMove = false;

	// Now check if the directions you want to go are valid moves
	if (movementDifferential.Y > checkingAccuracy && canMoveDown)
		validVerticalMove = true;
	else if (movementDifferential.Y < -checkingAccuracy && canMoveUp)
		validVerticalMove = true;

	if (movementDifferential.X > checkingAccuracy && canMoveRight)
		validHorizontalMove = true;
	else if (movementDifferential.X < -checkingAccuracy && canMoveLeft)
		validHorizontalMove = true;

	// If you have two valid axis of movement then choose the axis that has the largest distance remaining
	if (validHorizontalMove && validVerticalMove)
	{
		// Then choose the option that minimses the distance
		if (abs(movementDifferential.X) > abs(movementDifferential.Y))
		{
			if (movementDifferential.X > checkingAccuracy && mCurrentFacingDirection != FACING_DIRECTION::LEFT)
			{
				mCurrentFacingDirection = FACING_DIRECTION::RIGHT;
			}
			else if (movementDifferential.X < -checkingAccuracy && mCurrentFacingDirection != FACING_DIRECTION::RIGHT)
			{
				mCurrentFacingDirection = FACING_DIRECTION::LEFT;
			}
		}
		else
		{
			if (movementDifferential.Y > checkingAccuracy && mCurrentFacingDirection != FACING_DIRECTION::UP)
			{
				mCurrentFacingDirection = FACING_DIRECTION::DOWN;
			}
			else if (movementDifferential.Y < -checkingAccuracy && mCurrentFacingDirection != FACING_DIRECTION::DOWN)
			{
				mCurrentFacingDirection = FACING_DIRECTION::UP;
			}
		}
	}
	else if (validHorizontalMove || validVerticalMove) // If both options are not valid, but one of them is then move in the valid direction
	{
		if (movementDifferential.X > checkingAccuracy && canMoveRight && mCurrentFacingDirection != FACING_DIRECTION::LEFT)
			mCurrentFacingDirection = FACING_DIRECTION::RIGHT;
		else if (movementDifferential.X < -checkingAccuracy && canMoveLeft && mCurrentFacingDirection != FACING_DIRECTION::RIGHT)
			mCurrentFacingDirection = FACING_DIRECTION::LEFT;
		else if (movementDifferential.Y > checkingAccuracy && canMoveDown && mCurrentFacingDirection != FACING_DIRECTION::UP)
			mCurrentFacingDirection = FACING_DIRECTION::DOWN;
		else if (movementDifferential.Y < -checkingAccuracy && canMoveUp && mCurrentFacingDirection != FACING_DIRECTION::DOWN)
			mCurrentFacingDirection = FACING_DIRECTION::UP;
	}
	else // If neither of the desired moves are valid then just move in a valid direction
	{
		if (canMoveDown && mCurrentFacingDirection != FACING_DIRECTION::UP)
			mCurrentFacingDirection = FACING_DIRECTION::DOWN;
		else if (canMoveUp && mCurrentFacingDirection != FACING_DIRECTION::DOWN)
			mCurrentFacingDirection = FACING_DIRECTION::UP;
		else if (canMoveLeft && mCurrentFacingDirection != FACING_DIRECTION::RIGHT)
			mCurrentFacingDirection = FACING_DIRECTION::LEFT;
		else if (canMoveRight && mCurrentFacingDirection != FACING_DIRECTION::LEFT)
			mCurrentFacingDirection = FACING_DIRECTION::RIGHT;
	}
}

// -------------------------------------------------------------------- //

S2D::Vector2 Ghost::ConvertPositionToGridPosition(const S2D::Vector2 position)
{
	return S2D::Vector2(position.X / SPRITE_RESOLUTION, position.Y / SPRITE_RESOLUTION);
}

// -------------------------------------------------------------------- //

void Ghost::MoveInCurrentDirection(const float deltaTime)
{
	// Variables that will be needed in this function
	S2D::Vector2 gridPos               = S2D::Vector2(), movementAmount = S2D::Vector2();
	float        ghostMovementDistance = (GHOST_MOVEMENT_SPEED * deltaTime) / SPRITE_RESOLUTION;

	// First lock the opposite axis to which we are moving in
	if (mCurrentFacingDirection == FACING_DIRECTION::DOWN || mCurrentFacingDirection == FACING_DIRECTION::UP)
		mPosition.X = ((int)mPosition.X + 0.5f);
	else if (mCurrentFacingDirection == FACING_DIRECTION::LEFT || mCurrentFacingDirection == FACING_DIRECTION::RIGHT)
		mPosition.Y = ((int)mPosition.Y + 0.5f);

	float checkDistance = (mSingleSpriteHeight * 0.4f) / SPRITE_RESOLUTION;

	// Now we need to move the ghost in the direction selected, if we can
	switch (mCurrentFacingDirection)
	{

	case FACING_DIRECTION::DOWN:
		// Store the projected position into the grid pos variable for furture comparisons
		gridPos          = mPosition + S2D::Vector2(0.0f, checkDistance);

		movementAmount.Y = ghostMovementDistance;
	break;

	case FACING_DIRECTION::UP:
		// Store the projected position into the grid pos variable for furture comparisons
		gridPos          = mPosition + S2D::Vector2(0.0f, -1.0f * checkDistance);

		movementAmount.Y = -1.0f * ghostMovementDistance;
	break;

	case FACING_DIRECTION::LEFT:
		// Store the projected position into the grid pos variable for furture comparisons
		gridPos          = mPosition + S2D::Vector2(-1.0f * checkDistance, 0.0f);

		movementAmount.X = -1.0f * ghostMovementDistance;
	break;

	case FACING_DIRECTION::RIGHT:
		// Store the projected position into the grid pos variable for furture comparisons
		gridPos          = mPosition + S2D::Vector2(checkDistance, 0.0f);

		movementAmount.X = ghostMovementDistance;
	break;

	case FACING_DIRECTION::NONE:
	break;

	default:
		std::cout << "In an error movement state!" << std::endl;
		return;
	break;
	}

	// Check if we can acually go in this direction without going into a wall
	if (mCollisionMap[(unsigned int)gridPos.Y][(unsigned int)gridPos.X] != '1')
	{
		// if so move
		mPosition.X += movementAmount.X;
		mPosition.Y += movementAmount.Y;
	}
	else
	{
		// otherwise lock the position to being the centre of the current grid square
		if (movementAmount.X > 0)
			mPosition.X = ((int)mPosition.X + 0.5f);
		else
			mPosition.Y = ((int)mPosition.Y + 0.5f);

		// Reset the facing direction to being nothing, so we dont continue to run this code needlessly
		mCurrentFacingDirection = FACING_DIRECTION::NONE;
	}
}

// -------------------------------------------------------------------- //

void Ghost::LoadInSpriteSheets(const char* filePathForColored, const char* filePathForFlee)
{
	if (!mColouredSpriteSheet)
	{
		mColouredSpriteSheet = new S2D::Texture2D();
		mColouredSpriteSheet->Load(filePathForColored, false);

		if (!mColouredSpriteSheet)
		{
			std::cout << "Failed to load in the ghost sprite sheet" << std::endl;
			return;
		}
	}

	if (!mFleeSpriteSheet)
	{
		mFleeSpriteSheet = new S2D::Texture2D();
		mFleeSpriteSheet->Load(filePathForFlee, false);

		if (!mFleeSpriteSheet)
		{
			std::cout << "Failed to load in the ghost sprite sheet" << std::endl;
			return;
		}
	}
}

// -------------------------------------------------------------------- //