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

#include "Stack_FiniteStateMachine.h"

class Ghost final
{
public:
	Ghost() = delete; // Make it so you cannot create a default ghost type
	Ghost(S2D::Vector2 startPos, char** collisionMap, GHOST_TYPE ghostType, bool isAIControlled, char* FilePath, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight);
	~Ghost();

	void Render();
	void Update(const float deltaTime, S2D::Vector2 pacmanPos);

	void SetIsPlayerControlled()                       { mIsPlayerControlled = true; }

	Stack_FiniteStateMachine_Ghost*  GetStateMachine() { return mStateMachine; }

	bool IsEaten()                                     { return !mIsGhostAlive; }
	void SetGhostIsAlive(bool newVal)                  { mIsGhostAlive = newVal; }

	bool GetIsHome()                                   { return mIsHome; }
	void SetIsHome(bool newVal)                        { mIsHome = newVal; }

	void SetTargetPosition(S2D::Vector2 newPos)        { mTargetPositon = newPos; }

private:
	S2D::Vector2                     CalculateTargetPosition(S2D::Vector2 pacmanGridPos, FACING_DIRECTION pacmanFacingDirection);
	void                             LoadInSpriteSheet(char* filePath);

	void                             MoveInCurrentDirection(const float deltaTime);
	S2D::Vector2                     ConvertPositionToGridPosition(S2D::Vector2 position);
	 
	S2D::Vector2                     mPosition;
	S2D::Vector2                     mTargetPositon;

	static S2D::Texture2D*           mSpriteSheet; 
	static S2D::Rect		         mSourceRect;
	static S2D::Rect		         mDestRect;

	Stack_FiniteStateMachine_Ghost*   mStateMachine; // Each ghost has a state machine if it is AI controlled

	const unsigned int               mSpritesOnWidth;
	const unsigned int               mSpritesOnHeight;
	static unsigned int              mSingleSpriteWidth;
	static unsigned int              mSingleSpriteHeight;
	static char**                    mCollisionMap;

	FACING_DIRECTION                 mCurrentFacingDirection;
	FACING_DIRECTION                 mRequestedFacingDirection;

	unsigned int		             mCurrentFrame;
	unsigned int		             mStartFrame;
	unsigned int		             mEndFrame;

	GHOST_TYPE			             mThisGhostType;
	bool							 mIsPlayerControlled;
	bool                             mIsGhostAlive;
	bool                             mIsHome;

};



#endif