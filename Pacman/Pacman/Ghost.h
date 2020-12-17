#ifndef _GHOST_H_
#define _GHOST_H_

#include "S2D/S2D.h"
#include "Commons.h"

#include "BaseCharacter.h"

// ------------------------------------------------------------------------------- //

#include "Stack_FiniteStateMachine.h"

class Ghost final : public BaseCharacter
{
public:
	Ghost() = delete; // Make it so you cannot create a default ghost type
	Ghost(const S2D::Vector2 startPos,
		  char** const       collisionMap,
		  const GHOST_TYPE   ghostType,
		  const bool		 isAIControlled,
		  const char*        filePathForColoured,
		  const char*        filePathForFlee,
		  const unsigned int spritesOnWidthMain,
		  const unsigned int spritesOnHeightMain,
		  const unsigned int spritesOnWidthAlternate,
		  const unsigned int spritesOnHeightAlternate,
		  const bool         startsAtHome);
	~Ghost() override;

	void Render(const unsigned int frameCount) override;
	void Update(const float deltaTime, const S2D::Vector2 pacmanPos, const FACING_DIRECTION pacmanFacingDirection);

	Stack_FiniteStateMachine_Ghost*  GetStateMachine() { return mStateMachine; }


	bool GetIsHome()                             const { return mIsHome; }
	void SetIsHome(bool newVal)                        { mIsHome = newVal; }

	bool GetCanLeaveHome()                       const { return mCanLeaveHome; }
	void SetCanLeaveHome(bool newVal)                  { mCanLeaveHome = newVal; }

	void SetGhostIsFleeing(bool newVal);

	void SetGhostIsEaten(bool newVal);
	bool GetIfGhostIsEaten()                    const { return mGhostIsEaten; }

	void SetGhostsShouldReset();

private:
	bool                             CanTurnToDirection(const FACING_DIRECTION direction) override;

	void                             CheckForDirectionChange() override; // This function is for setting start and end frames for specific directional sprites

	void							 ResetGhostFromDeath();

	Stack_FiniteStateMachine_Ghost*  mStateMachine; // Each ghost has a state machine if it is AI controlled

	unsigned int                     mColourBaseStartFrame;
	unsigned int                     mColourBaseEndFrame;

	GHOST_TYPE			             mThisGhostType;
	bool                             mIsHome;
	bool                             mGhostIsFleeing;
	bool                             mGhostIsEaten;

	bool							 mCanLeaveHome;
	bool                             mResetting;
	bool                             mStartAtHome;

};



#endif