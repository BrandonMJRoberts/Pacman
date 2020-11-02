#ifndef _PACMAN_CHARACTER_H_
#define _PACMAN_CHARACTER_H_

#include "S2D/S2D.h"

#include "Commons.h"

class PacmanCharacter final
{
public:
	PacmanCharacter(char** collisionMap, const unsigned int spritesOnWidth = 3, const unsigned int spritesOnHeight = 4);
	~PacmanCharacter();

	void             Render(const unsigned int currentFrameCount);
	void             Update(const float deltaTime);

	S2D::Vector2     GetCentrePosition()      { return mCentrePosition; }

	S2D::Texture2D*  GetTexture()             { return mPacmanTexture; }
	S2D::Rect*       GetRect()                { return mPacmanSourceRect; }

	FACING_DIRECTION GetFacingDirection()     { return mCurrentFacingDirection; }

	void             ResetCharacter();

private:
	void             HandleInput();
	bool             EdgeCheck();
	void             MoveInCurrentDirection(const float deltaTime);
	void             CheckForDirectionChange();

	bool             CanMoveInDirection(FACING_DIRECTION direction);

	void             ReSetupPacmanSourceRect(float x, float y, unsigned int width, unsigned int height);

	S2D::Vector2     ConvertPositionToGridPosition(S2D::Vector2 position);

	char**           mCollisionMap;

	S2D::Vector2     mRenderPosition;
	S2D::Vector2     mCentrePosition;

	S2D::Vector2     mStartPosition;

	S2D::Texture2D*  mPacmanTexture;
	S2D::Rect*       mPacmanSourceRect; // Required for the sprite batch rendering

	unsigned int	 mSingleSpriteWidth;
	unsigned int	 mSingleSpriteHeight;

	const unsigned int mSpritesOnWidth;
	const unsigned int mSpritesOnHeight;

	unsigned int     mCurrentFrame;
	unsigned int     mStartFrame;
	unsigned int     mEndFrame;

	float            mChangeDirectionInputDelay;

	FACING_DIRECTION mCurrentFacingDirection;
	FACING_DIRECTION mRequestedFacingDirection;
};

#endif

