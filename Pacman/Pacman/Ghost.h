#ifndef _GHOST_H_
#define _GHOST_H_

#include "S2D/S2D.h"
#include "Commons.h"

// ------------------------------------------------------------------------------- //

#include "Stack_FiniteStateMachine.h"

class Ghost final
{
public:
	Ghost() = delete; // Make it so you cannot create a default ghost type
	Ghost(const S2D::Vector2 startPos, char** collisionMap, const GHOST_TYPE ghostType, const bool isAIControlled, const char* FilePathForColoured, const char* FilePathForFlee, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight);
	~Ghost();

	void Render(const unsigned int currentFrameCount);
	void Update(const float deltaTime, const S2D::Vector2 pacmanPos, const FACING_DIRECTION pacmanFacingDirection);

	void SetIsPlayerControlled()                       { mIsPlayerControlled = true; }

	Stack_FiniteStateMachine_Ghost*  GetStateMachine() { return mStateMachine; }

	bool IsEaten()                                     { return !mIsGhostAlive; }
	void SetGhostIsAlive(bool newVal)                  { mIsGhostAlive = newVal; }

	bool GetIsHome()                                   { return mIsHome; }
	void SetIsHome(bool newVal)                        { mIsHome = newVal; }

	void SetGhostIsFleeing(bool newVal)                { mGhostIsFleeing = newVal; }

	void SetTargetPosition(S2D::Vector2 newPos)        { mTargetPositon = newPos; }

private:
	void                             LoadInSpriteSheets(const char* filePathForColored, const char* filePathForFlee);

	void                             MoveInCurrentDirection(const float deltaTime);
	S2D::Vector2                     ConvertPositionToGridPosition(const S2D::Vector2 position);

	bool							 CanTurnToDirection(const FACING_DIRECTION newDir);
	void							 CalculateAIMovementDirection();

	S2D::Vector2                     mPosition;
	S2D::Vector2                     mTargetPositon;

	static S2D::Texture2D*           mColouredSpriteSheet; 
	static S2D::Texture2D*           mFleeSpriteSheet;
	static S2D::Rect		         mSourceRect;

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

	const float						 mTimePerChangeOfDirection;
	float							 mTimePerChangeDirectionRemaining;

	GHOST_TYPE			             mThisGhostType;
	bool							 mIsPlayerControlled;
	bool                             mIsGhostAlive;
	bool                             mIsHome;
	bool                             mGhostIsFleeing;

};



#endif