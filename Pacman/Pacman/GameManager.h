#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "S2D/S2D.h"
#include "Commons.h"
#include "Constants.h"

#include <string>

class GameManager final
{
public:
	~GameManager();
	  
	// Static instance
	static GameManager*   Instance();
	 
	void                  Update(const float deltaTime);

	// Dot count
	unsigned int          GetRemainingDots()			                      { return mRemainingDots; }
	void                  SetCurrentDotCount(const unsigned int count)        { mRemainingDots = count; }
	void                  DecreaseRemainingDots(const unsigned int amount)    { mRemainingDots -= amount; }

	// Levels
	void                  SetCurrentLevel(unsigned int newID)                 { mCurrentLevelID = newID; }
	unsigned int          GetCurrentLevel()                                   { return mCurrentLevelID; }

	// Scores
	void                  AddToScore(unsigned int addition)                   { mCurrentScore += addition;  mPointsRemainingTillNextLife -= addition; }
	unsigned int          GetCurrentScore()			                          { return mCurrentScore; }
	void                  SetCurrentScore(unsigned int newVal)                { mCurrentScore = newVal; }
	void                  ResetScoreForExtraLife()                            { mPointsRemainingTillNextLife = POINTS_PER_EXTRA_LIFE; }

	// Highscores
	unsigned int          SetCurrentHighScore(unsigned int score)             { mCurrentHighScore = score; }
	unsigned int          GetCurrentHighScore()                               { return mCurrentHighScore; }

	// Powered up
	void                  SetPlayerPoweredUp(bool playerState);
	bool                  GetIsPlayerPoweredUp()                              { return mPlayerIsPoweredUp; }

	// Grid offset
	S2D::Vector2          GetGridOffset()                                     { return mGridOffsetFromTopLeft; }

	// Alive state
	bool                  GetPlayerIsAlive()					              { return mPlayerIsAlive; }
	void                  SetPlayerIsAlive(bool newVal)                       { mPlayerIsAlive = newVal; }

	// Pause
	bool                  GetGameIsPaused()                                   { return mGameIsPaused; }
	void                  SetGameIsPaused(bool newVal)                        { mGameIsPaused = newVal; }
	bool                  GetIsAlreadySettingGamePaused()                     {  return mSettingGamePausedState; }
	void                  SetIsPausedButtonPressed(bool newVal)               { mSettingGamePausedState = newVal; }

	// Player character type
	PLAYER_CHARACTER_TYPE GetPlayerCharacterType()                            { return mPlayerCharacterType; }
	void                  SetPlayerCharacter(PLAYER_CHARACTER_TYPE newType)   { mPlayerCharacterType = newType; }
	void                  IncrementPlayerCharacter();

	// Extra lives
	unsigned int          GetExtraLivesCount()                                { return mExtraLifeCount; }
	void                  AddExtraLife()                                      { mExtraLifeCount++; }
	void                  RemoveLife()                                        { if (mExtraLifeCount > 0) mExtraLifeCount--; }
	void                  SetCurrentExtraLifeCount(unsigned int newVal)       { mExtraLifeCount = newVal; }
	 
	// Collectables
	PICKUP_TYPES          GetThisLevelCollectableType()                       { return mThisLevelCollectableSpawnType; }
	void                  SetLevelPickupType(PICKUP_TYPES newType)            { mThisLevelCollectableSpawnType = newType; }

	// Loading/restarting levels
	void                  RestartLevel();
	void                  LoadLevel(const unsigned int newLevelID);

	// Player name
	string                GetPlayerName()                                     { return mPlayerName; }
	void                  SetPlayerName(string newName)                       { mPlayerName = newName; }

	// Player name colour index
	unsigned int          GetPlayerNameColourIndex()                          { return mPlayerNameColourIndex; }
	void                  SetPlayerColourIndex(unsigned int newVal)           { mPlayerNameColourIndex = newVal % 7; }
	void                  IncrementPlayerColourIndex()                        { mPlayerNameColourIndex = (mPlayerNameColourIndex + 1) % 7; }

	// Save score
	void                  SaveOutScoreToLeaderboard();
private:
	GameManager();
	void                SetDotCounts();
	void                SetVariablesToStartingValues();

	// static instance
	static GameManager* mInstance;

	S2D::Vector2        mGridOffsetFromTopLeft;

	// Id's / counters
	unsigned int		mCurrentLevelID;
	unsigned int        mCurrentScore;
	unsigned int        mCurrentHighScore;
	unsigned int        mExtraLifeCount;
	unsigned int	    mRemainingDots;

	int                 mPointsRemainingTillNextLife;
	float               mTimeRemainingInPoweredUpState;

	// Player name data
	string              mPlayerName;
	unsigned int        mPlayerNameColourIndex;

	// Character type
	PLAYER_CHARACTER_TYPE   mPlayerCharacterType;

	// Collectables
	PICKUP_TYPES        mThisLevelCollectableSpawnType;

	// Player state
	bool                mPlayerIsPoweredUp;
	bool			    mPlayerIsAlive;

	// Game state
	bool                mGameIsPaused;
	bool                mSettingGamePausedState;
};

#endif // !_GAME_MANAGER_H_
