#ifndef _GHOST_H_
#define _GHOST_H_

#include "S2D/S2D.h"
#include "Commons.h"

// ------------------------------------------------------------------------------- //

enum class GHOST_TYPE : char
{
	RED,
	BLUE,
	PINK,
	ORANGE
};

// ------------------------------------------------------------------------------- //

class Ghost final
{
public:
	Ghost() = delete; // Make it so you cannot create a default ghost type
	Ghost(S2D::Vector2 startPos, char** collisionMap, GHOST_TYPE ghostType, char* FilePath, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight);
	~Ghost();

	void Render();
	void Update(const float deltaTime, S2D::Vector2 pacmanPos);

private:
	S2D::Vector2                     CalculateTargetPosition(S2D::Vector2 pacmanGridPos, FACING_DIRECTION pacmanFacingDirection);
	void                             LoadInSpriteSheet(char* filePath);
	 
	S2D::Vector2                     mPosition;
	S2D::Vector2                     mTargetPositon;

	static S2D::Texture2D*           mSpriteSheet; 
	static S2D::Rect		         mSourceRect;
	static S2D::Rect		         mDestRect;

	const unsigned int               mSpritesOnWidth;
	const unsigned int               mSpritesOnHeight;
	static unsigned int              mSingleSpriteWidth;
	static unsigned int              mSingleSpriteHeight;
	static char**                    mCollisionMap;

	unsigned int		             mCurrentFrame;
	unsigned int		             mStartFrame;
	unsigned int		             mEndFrame;

	GHOST_TYPE			             mThisGhostType;

};



#endif