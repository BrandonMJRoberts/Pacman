#include "GameManager.h"
#include "Constants.h"
#include "Commons.h"
#include "HighScoresMenu.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "UIManager.h"

GameManager* GameManager::mInstance = nullptr;

// ---------------------------------------------------------------- //

GameManager::GameManager()
{
	mCurrentLevelID				   = 0;

	mRemainingDots				   = 0;

	mExtraLifeCount                = 2;
	mPlayerIsPoweredUp             = false;
	mCurrentScore                  = 0;
	mCurrentHighScore              = 0;

	mTimeRemainingInPoweredUpState = 0.0f;

	mGridOffsetFromTopLeft         = S2D::Vector2(0.0f, 100.0f);

	mPlayerIsAlive                 = true;
	mGameIsPaused				   = false;

	mPlayerCharacterType           = PLAYER_CHARACTER_TYPE::PACMAN;

	mThisLevelCollectableSpawnType = PICKUP_TYPES::CHERRY;
	UIManager::GetInstance()->AddCollectedPickup(mThisLevelCollectableSpawnType);
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
	// If no dots remain then the player has completed the level and we need to move onto the next level
	if (mRemainingDots == 0)
	{
		LoadLevel(GetCurrentLevel() + 1);
		return;
	}

	if (mTimeRemainingInPoweredUpState > 0)
	{
		mTimeRemainingInPoweredUpState -= deltaTime;
	}

	if (mTimeRemainingInPoweredUpState <= 0 && mPlayerIsPoweredUp)
	{
		SetPlayerPoweredUp(false);
	}

	// Check if the current score has beaten the current highscore
	if (mCurrentScore > mCurrentHighScore)
		mCurrentHighScore = mCurrentScore;

	// You get an extra life after collecting 10,000 points
	if (mCurrentScore != 0)
	{
		if (mCurrentScore % POINTS_PER_EXTRA_LIFE == 0)
		{
			if (!mAllocatedExtraLife)
			{
				AddExtraLife();
				mAllocatedExtraLife = true;
			}
		}
		else
			mAllocatedExtraLife = false;
	}
}

// ---------------------------------------------------------------- //

void GameManager::LoadLevel(const unsigned int newLevelID)
{
	mCurrentLevelID = newLevelID % AMOUNT_OF_LEVELS;

	// Re-allocate a ramdom collectable type for this level
	mThisLevelCollectableSpawnType = (PICKUP_TYPES)(rand() % 8);

	UIManager::GetInstance()->AddCollectedPickup(mThisLevelCollectableSpawnType);
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

void GameManager::SaveOutScoreToLeaderboard()
{
	// First load in all of the scores currently saved out
	ifstream readFile("Save Data/HighScores.txt");

	if (!readFile.is_open())
	{
		std::cout << "Failed to open the highscores data file for saving out score!" << std::endl;
		return;
	}

	char*                     cLine = new char[100];
	std::vector<ScoreData>    scoreData;
	unsigned int              colourIndex, score;
	std::string               sLine, name;
	std::stringstream         ssLine;

	while (readFile.getline(cLine, 100))
	{
		sLine  = cLine;
		ssLine = std::stringstream(sLine);

		ssLine >> name >> score >> colourIndex;
		scoreData.push_back(ScoreData(score, name, colourIndex));
	}
	readFile.close();

	bool addedToList = false;

	// Now add the score into where it should go
	for (unsigned int i = 0; i < scoreData.size(); i++)
	{
		if (scoreData[i].GetScore() < mCurrentScore)
		{
			scoreData.insert(scoreData.begin() + i, ScoreData(mCurrentScore, "Test", 0));
			addedToList = true;
			break;
		}
	}

	if(!addedToList && scoreData.size() < 10)
		scoreData.push_back(ScoreData(mCurrentScore, "Test", 0));

	// Cap scores count to 10
	while (scoreData.size() > 10)
		scoreData.pop_back();

	// Save out the scores to the file
	ofstream writeFile("Save Data/HighScores.txt");

	if (!writeFile.is_open())
	{
		std::cout << "Failed to write to the highscores file" << std::endl;
		return;
	}

	for (unsigned int i = 0; i < scoreData.size(); i++)
		writeFile << scoreData[i].GetName() << " " << scoreData[i].GetScore() << " " << scoreData[i].GetColourIndex() << "\n";

	writeFile.close();

	delete[] cLine;
}

// ---------------------------------------------------------------- //