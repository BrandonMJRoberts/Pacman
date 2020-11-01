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

	void                AddToScore(unsigned int addition)    { mCurrentScore += addition;  mPointsRemainingTillNextLife -= addition; }
	unsigned int        GetCurrentScore()			         { return mCurrentScore; }

	unsigned int        SetCurrentHighScore(unsigned int score) { mCurrentHighScore = score; }
	unsigned int        GetCurrentHighScore()                { return mCurrentHighScore; }

	void                SetPlayerPoweredUp(bool playerState);
	bool                GetIsPlayerPoweredUp()               { return mPlayerIsPoweredUp; }

	S2D::Vector2        GetGridOffset()                      { return mGridOffsetFromTopLeft; }

	bool                GetPlayerIsAlive()					 { return mPlayerIsAlive; }
	void                SetPlayerIsAlive(bool newVal)        { mPlayerIsAlive = newVal; }

	bool                GetGameIsPaused()                    { return mGameIsPaused; }
	void                SetGameIsPaused(bool newVal)         { mGameIsPaused = newVal; }

	bool                GetIsAlreadySettingGamePaused()       { return mSettingGamePausedState; }
	void                SetIsPausedButtonPressed(bool newVal) { mSettingGamePausedState = newVal; }

	PLAYER_CHARACTER_TYPE GetPlayerCharacterType()                          { return mPlayerCharacterType; }
	void                  SetPlayerCharacter(PLAYER_CHARACTER_TYPE newType) { mPlayerCharacterType = newType; }
	void                  IncrementPlayerCharacter();

	unsigned int          GetExtraLivesCount()                         { return mExtraLifeCount; }
	void                  AddExtraLife()                               { if(mExtraLifeCount < 5) mExtraLifeCount++; }
	void                  RemoveLife()                                 { if (mExtraLifeCount > 0) mExtraLifeCount--; }

	PICKUP_TYPES          GetThisLevelCollectableType()                { return mThisLevelCollectableSpawnType; }

	void                  RestartLevel();
	void                  LoadLevel(const unsigned int newLevelID);

	void                  SaveOutScoreToLeaderboard();
private:
	GameManager();
	void                SetDotCounts();

	static GameManager* mInstance;

	S2D::Vector2        mGridOffsetFromTopLeft;

	unsigned int	    mRemainingDots;

	unsigned int		mCurrentLevelID;
	unsigned int        mCurrentScore;
	unsigned int        mCurrentHighScore;

	int        mPointsRemainingTillNextLife;

	unsigned int        mExtraLifeCount;

	float               mTimeRemainingInPoweredUpState;

	PLAYER_CHARACTER_TYPE   mPlayerCharacterType;

	PICKUP_TYPES        mThisLevelCollectableSpawnType;

	bool                mPlayerIsPoweredUp;
	bool			    mPlayerIsAlive;

	bool                mGameIsPaused;
	bool                mSettingGamePausedState;
};

#endif // !_GAME_MANAGER_H_
