#ifndef _BASE_CHARACTER_H_
#define _BASE_CHARACTER_H_

#include "S2D/S2D.h"
#include "Commons.h"

class BaseCharacter abstract
{
public:
	BaseCharacter() = delete;
	BaseCharacter(const char** const collisionMap,                    const S2D::Vector2 startPosition, 
		          const bool         isAIControlled,                  const char*        filePathForMainSpriteSheet, 
		          const char*        filePathForAlternateSpriteSheet, const unsigned int spritesOnWidth, 
		          const unsigned int spritesOnHeight,                 const unsigned int spritesOnWidthAlternate, const unsigned int spritesOnHeightAlternate);
	virtual          ~BaseCharacter();

	virtual void     Render(const unsigned int frameCount);
	virtual void     Update(const float deltaTime);

	void             SetIsPlayerControlled()                                   { mIsPlayerControlled = true; } 

	bool             IsAlive()                                           const { return mIsAlive; }
	virtual void     SetIsAlive(const bool newVal);

	void             SetTargetPosition(const S2D::Vector2 newPos)              { mTargetPositon = newPos; } 

	FACING_DIRECTION GetFacingDirection()                                const { return mCurrentFacingDirection; }
	S2D::Vector2     GetCentrePosition()                                 const { return mCentrePosition; }

	void             ResetCharacter();

protected:
	void                             HandleInput();
	bool                             EdgeCheck();
	bool                             CanMoveInDirection(FACING_DIRECTION direction);
	void                             MoveInCurrentDirection(const float deltaTime);
	bool							 CanTurnToDirection(const FACING_DIRECTION newDir);
	virtual	void                     CheckForDirectionChange(); // This function is for setting start and end frames for specific directional sprites

	S2D::Vector2                     mCentrePosition;        // Current position
	S2D::Vector2                     mStartPosition;         // Position to be reset to if the level restarts

	S2D::Vector2                     mTargetPositon;         // AI move to position
	S2D::Vector2                     mMoveToPos;

	S2D::Texture2D*                  mMainSpriteSheet;       // The default main sprite sheet
	S2D::Texture2D*                  mAlternateSpritesSheet; // The alternate sprite sheet
	S2D::Rect		                 mSourceRect;            // Portion of sprite sheet to be rendered

	const unsigned int               mSpritesOnWidthMain;
	const unsigned int               mSpritesOnHeightMain;
	const unsigned int               mSpritesOnWidthAlternate;
	const unsigned int               mSpritesOnHeightAlternate;

	unsigned int					 mSingleSpriteWidthMain;
	unsigned int					 mSingleSpriteHeightMain;
	unsigned int					 mSingleSpriteWidthAlternate;
	unsigned int					 mSingleSpriteHeightAlternate;

	const char**                     mCollisionMap;

	FACING_DIRECTION                 mCurrentFacingDirection;
	FACING_DIRECTION                 mRequestedFacingDirection;

	unsigned int		             mCurrentFrame;
	unsigned int		             mStartFrame;
	unsigned int		             mEndFrame;

	unsigned int                     mFramesPerAnimation;

	const float						 mTimePerChangeOfDirection;
	float							 mTimePerChangeDirectionRemaining;
	float							 mMovementSpeed;

	bool							 mIsPlayerControlled;
	bool                             mIsAlive;
	bool                             mUsingMainSpriteSheet;
};

#endif