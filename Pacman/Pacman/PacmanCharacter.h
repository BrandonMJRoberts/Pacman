#ifndef _PACMAN_CHARACTER_H_
#define _PACMAN_CHARACTER_H_

#include "S2D/S2D.h"

#include "Commons.h"

class PacmanCharacter
{
public:
	PacmanCharacter(char** collisionMap, unsigned int spritesOnWidth = 3, unsigned int spritesOnHeight = 4);
	~PacmanCharacter();

	void             Render(unsigned int currentFrameCount);
	void             Update(const float deltaTime);

	S2D::Vector2     GetTopLeftPosition()     { return mRenderPosition; }

	S2D::Texture2D*  GetTexture()             { return mPacmanTexture; }
	S2D::Rect*       GetRect()                { return mPacmanSourceRect; }

private:
	void             HandleInput();
	bool             EdgeCheck();
	void             MoveInCurrentDirection(const float deltaTime);
	void             CheckForDirectionChange();

	void             ReSetupPacmanSourceRect(float x, float y, int width, int height);

	S2D::Vector2     ConvertPositionToGridPosition(S2D::Vector2 position);

	char**           mCollisionMap;

	S2D::Vector2     mRenderPosition;
	S2D::Vector2     mCentrePosition;

	S2D::Texture2D*  mPacmanTexture;
	S2D::Rect*       mPacmanSourceRect; // Required for the sprite batch rendering

	unsigned int	 mSingleSpriteWidth;
	unsigned int	 mSingleSpriteHeight;

	float            mChangeDirectionInputDelay;

	FACING_DIRECTION mCurrentFacingDirection;
	FACING_DIRECTION mRequestedFacingDirection;
};

#endif

