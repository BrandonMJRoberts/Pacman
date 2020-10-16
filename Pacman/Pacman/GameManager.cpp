#include "GameManager.h"
#include "Constants.h"

GameManager* GameManager::mInstance = nullptr;

// ---------------------------------------------------------------- //

GameManager::GameManager()
{
	mCurrentLevelID				   = 0;
	mRemainingDots				   = 0;
	mStartingDots				   = 0;
	mPlayerIsPoweredUp             = false;
	mCurrentScore                  = 0;
	mTimeRemainingInPoweredUpState = 0.0f;
	mGridOffsetFromTopLeft         = S2D::Vector2(0.0f, 100.0f);
	mPlayerIsAlive                 = true;
	mGameIsPaused				   = false;
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
	mStartingDots  = 0;
}

// ---------------------------------------------------------------- //

void GameManager::RestartLevel()
{
	mRemainingDots = mStartingDots;
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
}

// ---------------------------------------------------------------- //