#ifndef _PACMAN_CHARACTER_H_
#define _PACMAN_CHARACTER_H_

#include "S2D/S2D.h"

#include "Commons.h"

#include "BaseState_Task_Pacman.h"
#include "BaseCharacter.h"

class Stack_FiniteStateMachine_Pacman;

class PacmanCharacter final : public BaseCharacter
{
public:
	PacmanCharacter() = delete;
	PacmanCharacter(const char** const collisionMap, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight, const S2D::Vector2 startPositon, const char* filePathForMainSpriteSheet, const char* filePathForAlternateSpriteSheet, bool isAIControlled);
	~PacmanCharacter() override;

	//void             Render(const unsigned int currentFrameCount) override;
	void             Update(const float deltaTime) override;

	//S2D::Vector2     GetCentrePosition()      { return mCentrePosition; }
	//S2D::Texture2D*  GetTexture()             { return mPacmanTexture; }
	//S2D::Rect*       GetRect()                { return mPacmanSourceRect; }
	//FACING_DIRECTION GetFacingDirection()     { return mCurrentFacingDirection; }

	//void             ResetCharacter();

private:
	// ----------------------------------------------------------------------------------------------------------------- //

	//void             HandleInput();

	void             ReSetupPacmanSourceRect(float x, float y, unsigned int width, unsigned int height);
	//bool             EdgeCheck();

	//bool             CanMoveInDirection(FACING_DIRECTION direction);
	//void             MoveInCurrentDirection(const float deltaTime);

	S2D::Vector2     ConvertPositionToGridPosition(S2D::Vector2 position);
	void             CheckForDirectionChange();

	void             UpdateAsPlayerControlled(const float deltaTime);
	void             UpdateAsAI();

	// ----------------------------------------------------------------------------------------------------------------- //

	//char**             mCollisionMap;

	//S2D::Vector2       mRenderPosition;
	//S2D::Vector2       mCentrePosition;

	//S2D::Vector2       mStartPosition;

	//S2D::Texture2D*    mPacmanTexture;
	//S2D::Rect*         mPacmanSourceRect; // Required for the sprite batch rendering

	//unsigned int	   mSingleSpriteWidth;
	//unsigned int	   mSingleSpriteHeight;

	//const unsigned int mSpritesOnWidth;
	//const unsigned int mSpritesOnHeight;

	//unsigned int       mCurrentFrame;
	//unsigned int       mStartFrame;
	//unsigned int       mEndFrame;

	//float              mChangeDirectionInputDelay;

	Stack_FiniteStateMachine_Pacman* mStateMachine;

	//FACING_DIRECTION   mCurrentFacingDirection;
	//FACING_DIRECTION   mRequestedFacingDirection;

	//bool               mIsAIControlled;
};

#endif

