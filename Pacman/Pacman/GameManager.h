#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "S2D/S2D.h"

class GameManager
{
public:
	~GameManager();

	static GameManager* Instance();

	void Update(const float deltaTime);

	unsigned int GetRemainingDots()			  { return mRemainingDots; }
	unsigned int GetStartingDots()			  { return mStartingDots; }

	void SetStartingLevel(unsigned int newID) { mCurrentLevelID = newID; SetDotCounts(); }
	unsigned int GetCurrentLevel()            { return mCurrentLevelID; }

	void AddToScore(unsigned int addition)    { mCurrentScore += addition;  }
	unsigned int GetCurrentScore()			  { return mCurrentScore; }

	void SetPlayerPoweredUp(bool playerState);
	bool GetIsPlayerPoweredUp()               { return mPlayerIsPoweredUp; }

	S2D::Vector2 GetGridOffset()              { return mGridOffsetFromTopLeft; }

	bool GetPlayerIsAlive()					  { return mPlayerIsAlive; }
	void SetPlayerIsAlive(bool newVal)        { mPlayerIsAlive = newVal; }

	bool GetGameIsPaused()                    { return mGameIsPaused; }
	void SetGameIsPaused(bool newVal)         { mGameIsPaused = newVal; }

	bool GetIsAlreadySettingGamePaused()       { return mSettingGamePausedState; }
	void SetIsPausedButtonPressed(bool newVal) { mSettingGamePausedState = newVal; }

	void RestartLevel();

private:
	GameManager();
	void SetDotCounts();

	static GameManager* mInstance;

	S2D::Vector2 mGridOffsetFromTopLeft;

	unsigned int	    mRemainingDots;
	unsigned int		mStartingDots;

	unsigned int		mCurrentLevelID;
	unsigned int        mCurrentScore;

	float               mTimeRemainingInPoweredUpState;

	bool                mPlayerIsPoweredUp;
	bool			    mPlayerIsAlive;

	bool                mGameIsPaused;
	bool                mSettingGamePausedState;
};

#endif // !_GAME_MANAGER_H_
