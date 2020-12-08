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
	unsigned int          GetRemainingDots()			                const { return mRemainingDots; }
	void                  SetCurrentDotCount(const unsigned int count)        { mRemainingDots = count; }
	void                  DecreaseRemainingDots(const unsigned int amount)    { mRemainingDots -= amount; }

	// Dots collected and ghosts eaten
	unsigned int         GetCurrentAmountOfDotsEaten()                  const { return mDotsCollectedThisGame; }
	void                 IncrementDotsEatenCount()                            { mDotsCollectedThisGame++; }
	void                 ResetDotsEatenCount()                                { mDotsCollectedThisGame = 0; }

	unsigned int         GetAmountOfGhostsEatenthisGame()               const { return mGhostsEatenThisGame; }
	void                 IncrementGhostsEatenCount();

	unsigned int         GetAmountOfGhostsEatenStreak()                 const { return mGhostsEatenStreak; }
	void                 ResetGhostsEatenCount()                              { mGhostsEatenThisGame = 0; }

	// Levels
	void                  SetCurrentLevel(unsigned int newID)                 { mCurrentLevelID = newID; }
	unsigned int          GetCurrentLevel()                             const { return mCurrentLevelID; }

	// Scores
	void                  AddToScore(unsigned int addition)                   { mCurrentScore += addition;  mPointsRemainingTillNextLife -= addition; }
	unsigned int          GetCurrentScore()			                  const   { return mCurrentScore; }
	void                  SetCurrentScore(unsigned int newVal)                { mCurrentScore = newVal; }
	void                  ResetScoreForExtraLife();

	// Highscores
	unsigned int          SetCurrentHighScore(unsigned int score)             { mCurrentHighScore = score; }
	unsigned int          GetCurrentHighScore()                         const { return mCurrentHighScore; }

	// Powered up
	void                  SetPlayerPoweredUp(bool playerState);
	bool                  GetIsPlayerPoweredUp()                        const { return mPlayerIsPoweredUp; }

	// Grid offset
	S2D::Vector2          GetGridOffset()                              const { return mGridOffsetFromTopLeft; }

	// Alive state
	bool                  GetPlayerIsAlive()					        const { return mPlayerIsAlive; }
	void                  SetPlayerIsAlive(bool newVal)                       { mPlayerIsAlive = newVal; }

	// Pause
	bool                  GetGameIsPaused()                             const { return mGameIsPaused; }
	void                  SetGameIsPaused(bool newVal)                        { mGameIsPaused = newVal; }
	bool                  GetIsAlreadySettingGamePaused()               const {  return mSettingGamePausedState; }
	void                  SetIsPausedButtonPressed(bool newVal)               { mSettingGamePausedState = newVal; }

	// Player character type
	PLAYER_CHARACTER_TYPE GetPlayerCharacterType()                      const { return mPlayerCharacterType; }
	void                  SetPlayerCharacter(PLAYER_CHARACTER_TYPE newType)   { mPlayerCharacterType = newType; }
	void                  IncrementPlayerCharacter();

	// Extra lives
	int                   GetExtraLivesCount()                          const { return mExtraLifeCount; }
	void                  AddExtraLife()                                      { mExtraLifeCount++; }
	void                  RemoveLife()                                        { mExtraLifeCount--; }
	void                  SetCurrentExtraLifeCount(unsigned int newVal)       { mExtraLifeCount = newVal; }
	 
	// Collectables
	PICKUP_TYPES          GetThisLevelCollectableType()                 const { return mThisLevelCollectableSpawnType; }
	void                  SetLevelPickupType(PICKUP_TYPES newType)            { mThisLevelCollectableSpawnType = newType; }

	// Loading/restarting levels
	void                  RestartLevel();
	void                  LoadLevel(const unsigned int newLevelID);

	// Player name
	string                GetPlayerName()                               const { return mPlayerName; }
	void                  SetPlayerName(string newName)                       { mPlayerName = newName; }

	// Player name colour index
	unsigned int          GetPlayerNameColourIndex()                    const { return mPlayerNameColourIndex; }
	void                  SetPlayerColourIndex(unsigned int newVal)           { mPlayerNameColourIndex = newVal % 7; }
	void                  IncrementPlayerColourIndex()                        { mPlayerNameColourIndex = (mPlayerNameColourIndex + 1) % 7; }

	// Save score
	void                  SaveOutScoreToLeaderboard();

	void                  IncreasePacmanDeathCounter()                        { ++mAmountOfPacmanDeathsThisGame; }
	unsigned int          GetAmountOfPacmanDeathsThisGame()             const { return mAmountOfPacmanDeathsThisGame; }

	bool                  GetIsInPreGameState()                         const { return mInPreGameState; }
	void                  ResetPreGameTimer()                                 { mTimeRemainingInPreGameState = TIME_IN_PRE_GAME; mInPreGameState = true; }

	unsigned int          GetAmountOfGhostsReleased()                   const { return mAmountOfGhostsReleased; }
	void                  ResetAmountOfGhostsReleased()                       { mAmountOfGhostsReleased = 1; }
	void                  IncreaseAmountOfGhostsReleased()                    { mAmountOfGhostsReleased++; }

private:
	GameManager();
	void                SetDotCounts();
	unsigned int        LoadInCurrentHighScore();
	void                SetVariablesToStartingValues(bool resettingLevel);

	// static instance
	static GameManager* mInstance;

	S2D::Vector2        mGridOffsetFromTopLeft;

	// Id's / counters
	unsigned int		mCurrentLevelID;
	unsigned int        mCurrentScore;
	unsigned int        mCurrentHighScore;
	int                 mExtraLifeCount;
	unsigned int	    mRemainingDots;

	unsigned int        mAmountOfGhostsReleased;

	unsigned int        mDotsCollectedThisGame;
	unsigned int        mGhostsEatenThisGame;
	unsigned int        mGhostsEatenStreak; // For points calculations

	unsigned int        mAmountOfPacmanDeathsThisGame;

	int                 mPointsRemainingTillNextLife;
	float               mTimeRemainingInPoweredUpState;

	float               mTimeRemainingInPreGameState;

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

	bool                mInPreGameState;
};

#endif // !_GAME_MANAGER_H_
