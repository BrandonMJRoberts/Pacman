#ifndef _PACMAN_CHARACTER_H_
#define _PACMAN_CHARACTER_H_

#include "S2D/S2D.h"

#include "Commons.h"

class PacmanCharacter
{
public:
	PacmanCharacter(char** collisionMap, unsigned int collisionMapWidth, unsigned int collisionMapHeight);
	~PacmanCharacter();

	void Render(unsigned int currentFrameCount);
	void Update(const float deltaTime);

	S2D::Vector2    GetPosition(){ return mPosition; }
	S2D::Texture2D* GetTexture() { return mPacmanTexture; }
	S2D::Rect*      GetRect()    { return mPacmanSourceRect; }

private:
	void HandleInput(const float deltaTime);
	bool EdgeCheck();
	void MoveInCurrentDirection(const float deltaTime);
	void CheckForDirectionChange();

	char**          mCollisionMap;
	unsigned int	mCollisionWidth;
	unsigned int	mCollisionHeight;

	S2D::Vector2     mPosition;
	S2D::Texture2D*  mPacmanTexture;
	S2D::Rect*       mPacmanSourceRect; // Required for the sprite batch rendering

	FACING_DIRECTION mCurrentFacingDirection;
	FACING_DIRECTION mRequestedFacingDirection;

	unsigned int	 mSpriteWidth;
	unsigned int	 mSpriteHeight;

	unsigned int	 mSingleSpriteWidth;
	unsigned int	 mSingleSpriteHeight;
};

#endif

