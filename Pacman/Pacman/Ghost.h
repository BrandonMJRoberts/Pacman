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
	Ghost(const S2D::Vector2 startPos, const char** const collisionMap, const GHOST_TYPE ghostType, const bool isAIControlled, const char* FilePathForColoured, const char* FilePathForFlee, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight);
	~Ghost() override;

	void Update(const float deltaTime, const S2D::Vector2 pacmanPos, const FACING_DIRECTION pacmanFacingDirection);

	Stack_FiniteStateMachine_Ghost*  GetStateMachine() { return mStateMachine; }


	bool GetIsHome()                                   { return mIsHome; }
	void SetIsHome(bool newVal)                        { mIsHome = newVal; }

	void SetGhostIsFleeing(bool newVal)                { mGhostIsFleeing = newVal; }

private:
	void CheckForDirectionChange() override; // This function is for setting start and end frames for specific directional sprites

	void							 CalculateAIMovementDirection();

	Stack_FiniteStateMachine_Ghost*   mStateMachine; // Each ghost has a state machine if it is AI controlled

	GHOST_TYPE			             mThisGhostType;
	bool                             mIsHome;
	bool                             mGhostIsFleeing;

};



#endif