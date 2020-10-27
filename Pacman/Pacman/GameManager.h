#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "S2D/S2D.h"
#include "Commons.h"

class GameManager final
{
public:
	~GameManager();

	static GameManager* Instance();

	void                Update(const float deltaTime);

	unsigned int        GetRemainingDots()			         { return mRemainingDots; }

	void                SetCurrentDotCount(const unsigned int count)          { mRemainingDots = count; }
	void                DecreaseRemainingDots(const unsigned int amount)      { mRemainingDots -= amount; }

	void                SetStartingLevel(unsigned int newID) { mCurrentLevelID = newID; SetDotCounts(); }
	unsigned int        GetCurrentLevel()                    { return mCurrentLevelID; }

	void                AddToScore(unsigned int addition)    { mCurrentScore += addition;  }
	unsigned int        GetCurrentScore()			         { return mCurrentScore; }

	void                SetPlayerPoweredUp(bool playerState);
	bool                GetIsPlayerPoweredUp()               { return mPlayerIsPoweredUp; }

	S2D::Vector2        GetGridOffset()                      { return mGridOffsetFromTopLeft; }

	bool                GetPlayerIsAlive()					 { return mPlayerIsAlive; }
	void                SetPlayerIsAlive(bool newVal)        { mPlayerIsAlive = newVal; }

	bool                GetGameIsPaused()                    { return mGameIsPaused; }
	void                SetGameIsPaused(bool newVal)         { mGameIsPaused = newVal; }

	bool                GetIsAlreadySettingGamePaused()       { return mSettingGamePausedState; }
	void                SetIsPausedButtonPressed(bool newVal) { mSettingGamePausedState = newVal; }

	PLAYER_CHARACTER_TYPE GetPlayerCurrentCharacter()         { return mPlayerCharacterType; }
	void                  SetPlayerCharacter(PLAYER_CHARACTER_TYPE newType) { mPlayerCharacterType = newType; }
	void                  IncrementPlayerCharacter();

	unsigned int GetExtraLivesCount()                         { return mExtraLifeCount; }

	void                  RestartLevel();
	void                  LoadLevel(const unsigned int newLevelID);


private:
	GameManager();
	void                SetDotCounts();

	static GameManager* mInstance;

	S2D::Vector2        mGridOffsetFromTopLeft;

	unsigned int	    mRemainingDots;

	unsigned int		mCurrentLevelID;
	unsigned int        mCurrentScore;

	unsigned int        mExtraLifeCount;

	float               mTimeRemainingInPoweredUpState;

	PLAYER_CHARACTER_TYPE   mPlayerCharacterType;

	bool                mPlayerIsPoweredUp;
	bool			    mPlayerIsAlive;

	bool                mGameIsPaused;
	bool                mSettingGamePausedState;
};

#endif // !_GAME_MANAGER_H_
