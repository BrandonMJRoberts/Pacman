#ifndef _GHOST_H_
#define _GHOST_H_

#include "S2D/S2D.h"
#include "Commons.h"

#include "BaseCharacter.h"

// ------------------------------------------------------------------------------- //

#include "Stack_FiniteStateMachine.h"

// ------------------------------------------------------------------------------- //

struct GhostCreationData
{
	GhostCreationData(const S2D::Vector2 startPos,
		              char** const       collisionMap,
		              const GHOST_TYPE   ghostType,
		              const char*        filePathForColoured,
		              const char*        filePathForFlee,
		              const unsigned int spritesOnWidthMain,
		              const unsigned int spritesOnHeightMain,
		              const unsigned int spritesOnWidthAlternate,
		              const unsigned int spritesOnHeightAlternate,
		              const bool         startsAtHome) : mStartPos(startPos), 
		                                                 mCollisionMap(collisionMap),              
		                                                 mGhostType(ghostType), 
		                                                 mFilePathForColoured(filePathForColoured), 
		                                                 mFilePathForFlee(filePathForFlee),
		                                                 mSpritesOnWidthMain(spritesOnWidthMain),
														 mSpritesOnHeightMain(spritesOnHeightMain),
														 mSpritesOnHeightAlternate(spritesOnHeightAlternate),
														 mSpritesOnWidthAlternate(spritesOnWidthAlternate),
														 mStartsAtHome(startsAtHome)

	{

	}

	S2D::Vector2       mStartPos;
	char** const       mCollisionMap;
	GHOST_TYPE         mGhostType;
	const char*        mFilePathForColoured;
	const char*        mFilePathForFlee;
	const unsigned int mSpritesOnWidthMain;
	const unsigned int mSpritesOnHeightMain;
	const unsigned int mSpritesOnWidthAlternate;
	const unsigned int mSpritesOnHeightAlternate;
	bool               mStartsAtHome;

	~GhostCreationData() { ; }

};

// ------------------------------------------------------------------------------- //

class Ghost final : public BaseCharacter
{
public:
	Ghost() = delete; // Make it so you cannot create a default ghost type
	Ghost(GhostCreationData creationData, const bool isAIControlled);
	~Ghost() override;

	void Render(const unsigned int frameCount) override;
	void Update(const float deltaTime, const S2D::Vector2 pacmanPos, const FACING_DIRECTION pacmanFacingDirection);

	Stack_FiniteStateMachine_Ghost*  GetStateMachine() { return mStateMachine; }

	bool GetIsHome()                             const { return mIsHome; }
	void SetIsHome(bool newVal)                        { mIsHome = newVal; }

	bool GetCanLeaveHome()                       const { return mCanLeaveHome; }
	void SetCanLeaveHome(bool newVal)                  { mCanLeaveHome = newVal; }

	void SetGhostIsFleeing(bool newVal);
	bool GetGhostIsFleeing()                    const  { return mGhostIsFleeing; }

	void SetGhostIsEaten(bool newVal);
	bool GetIfGhostIsEaten()                    const { return mGhostIsEaten; }

	void SetGhostsShouldReset();
	void IncreaseGhostMovementSpeedToNextLevel();

private:
	bool                             CanTurnToDirection(const FACING_DIRECTION direction) override;

	void                             CheckForDirectionChange() override; // This function is for setting start and end frames for specific directional sprites

	void							 ResetGhostFromDeath();

	void                             HandleInput() override;

	void                             CalculateAIMovementDirection() override;

	const S2D::Vector2               mHomePosition;
	const S2D::Vector2               mHomeEntranceRightPosition;
	const S2D::Vector2               mHomeEntranceLeftPosition;

	Stack_FiniteStateMachine_Ghost*  mStateMachine; // Each ghost has a state machine if it is AI controlled

	unsigned int                     mColourBaseStartFrame;
	unsigned int                     mColourBaseEndFrame;

	float                            mGhostSetSpeed;

	GHOST_TYPE			             mThisGhostType;
	bool                             mIsHome;
	bool                             mGhostIsFleeing;
	bool                             mGhostIsEaten;

	bool							 mCanLeaveHome;
	bool                             mResetting;
	bool                             mStartAtHome;
};



#endif