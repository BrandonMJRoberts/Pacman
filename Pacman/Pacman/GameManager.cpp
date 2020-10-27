#include "GameManager.h"
#include "Constants.h"

GameManager* GameManager::mInstance = nullptr;

// ---------------------------------------------------------------- //

GameManager::GameManager()
{
	mCurrentLevelID				   = 0;

	mExtraLifeCount                = 2;

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
	mExtraLifeCount = 2;
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

void GameManager::IncrementPlayerCharacter()
{
	switch (mPlayerCharacterType)
	{
	case PLAYER_CHARACTER_TYPE::PACMAN:
		mPlayerCharacterType = PLAYER_CHARACTER_TYPE::BLUE_GHOST;
	break;

	case PLAYER_CHARACTER_TYPE::BLUE_GHOST:
		mPlayerCharacterType = PLAYER_CHARACTER_TYPE::ORANGE_GHOST;
	break;

	case PLAYER_CHARACTER_TYPE::ORANGE_GHOST:
		mPlayerCharacterType = PLAYER_CHARACTER_TYPE::RED_GHOST;
	break;

	case PLAYER_CHARACTER_TYPE::RED_GHOST:
		mPlayerCharacterType = PLAYER_CHARACTER_TYPE::PINK_GHOST;
	break;

	case PLAYER_CHARACTER_TYPE::PINK_GHOST:
		mPlayerCharacterType = PLAYER_CHARACTER_TYPE::PACMAN;
	break;

	default:
	break;

	}
}

// ---------------------------------------------------------------- //