#include "GameManager.h"
#include "Constants.h"

GameManager* GameManager::mInstance = nullptr;

// ---------------------------------------------------------------- //

GameManager::GameManager()
{
	mCurrentLevelID				   = 0;

	mRemainingDots				   = 0;

	mPlayerIsPoweredUp             = false;
	mCurrentScore                  = 0;
	mTimeRemainingInPoweredUpState = 0.0f;
	mGridOffsetFromTopLeft         = S2D::Vector2(0.0f, 100.0f);
	mPlayerIsAlive                 = true;
	mGameIsPaused				   = false;
	mPlayerCharacterType           = PLAYER_CHARACTER_TYPE::PACMAN;
}

// ---------------------------------------------------------------- //

GameManager::~GameManager()
{
	delete mInstance;
	mInstance = nullptr;
}

// ---------------------------------------------------------------- //

GameManager* GameManager::Instance()
{
	if (!mInstance)
	{
		mInstance = new GameManager();
	}

	return mInstance;
}

// ---------------------------------------------------------------- //

void GameManager::SetDotCounts()
{
	mRemainingDots = 0;
}

// ---------------------------------------------------------------- //

void GameManager::RestartLevel()
{
	
}

// ---------------------------------------------------------------- //

void GameManager::SetPlayerPoweredUp(bool playerState)
{
	if (playerState)
	{
		mTimeRemainingInPoweredUpState = TIME_IN_POWER_PELLET;
	}
	else
		mTimeRemainingInPoweredUpState = 0.0f;

	mPlayerIsPoweredUp = playerState; 
}

// ---------------------------------------------------------------- //

void GameManager::Update(const float deltaTime)
{
	if (mTimeRemainingInPoweredUpState > 0)
	{
		mTimeRemainingInPoweredUpState -= deltaTime;
	}

	if (mTimeRemainingInPoweredUpState <= 0 && mPlayerIsPoweredUp)
	{
		SetPlayerPoweredUp(false);
	}

	// If no dots remain then the player has completed the level and we need to move onto the next level
	if (mRemainingDots == 0)
	{
		LoadLevel(GetCurrentLevel() + 1);
	}
}

// ---------------------------------------------------------------- //

void GameManager::LoadLevel(const unsigned int newLevelID)
{
	mCurrentLevelID = newLevelID;
}

// ---------------------------------------------------------------- //