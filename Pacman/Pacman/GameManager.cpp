#include "GameManager.h"
#include "Constants.h"
#include "Commons.h"
#include "HighScoresMenu.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "UIManager.h"
#include "AudioManager.h"

GameManager* GameManager::mInstance = nullptr;

// ---------------------------------------------------------------- //

GameManager::GameManager()
{
	SetVariablesToStartingValues();
}

// ---------------------------------------------------------------- //

GameManager::~GameManager()
{
	// Clear up the memory allocated
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

void GameManager::SetVariablesToStartingValues()
{
	mPlayerName                    = "UNNAMED";
	mCurrentLevelID                = 0;
	mPlayerNameColourIndex         = 1;

	mDotsCollectedThisGame         = 0;
	mGhostsEatenThisGame           = 0;

	mRemainingDots                 = 0;

	mExtraLifeCount                = STARTING_LIFE_COUNT;
	mPlayerIsPoweredUp             = false;
	mCurrentScore                  = 0;
	mCurrentHighScore              = LoadInCurrentHighScore();

	mTimeRemainingInPoweredUpState = 0.0f;

	mGridOffsetFromTopLeft         = S2D::Vector2(0.0f, 100.0f);

	mPlayerIsAlive                 = true;
	mGameIsPaused                  = false;

	mPlayerCharacterType           = PLAYER_CHARACTER_TYPE::PACMAN;

	mThisLevelCollectableSpawnType = PICKUP_TYPES::CHERRY;

	mPointsRemainingTillNextLife = POINTS_PER_EXTRA_LIFE;

	mGhostsEatenStreak = 0;
}

// ---------------------------------------------------------------- //

void GameManager::SetDotCounts()
{
	mRemainingDots = 0;
}

// ---------------------------------------------------------------- //

void GameManager::RestartLevel()
{
	SetVariablesToStartingValues();
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
	// First handle the powerup timer
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
	if (mPointsRemainingTillNextLife <= 0)
	{
		if (mExtraLifeCount < 5)
		{
			AddExtraLife();
			AudioManager::GetInstance()->PlayExtraLifeSFX();
		}

		mPointsRemainingTillNextLife += POINTS_PER_EXTRA_LIFE;
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

	// Read in all scores currently
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
		if (scoreData[i].score < mCurrentScore)
		{
			if(mPlayerName != "")
				scoreData.insert(scoreData.begin() + i, ScoreData(mCurrentScore, mPlayerName, mPlayerNameColourIndex));
			else
				scoreData.insert(scoreData.begin() + i, ScoreData(mCurrentScore, "UNNAMED", mPlayerNameColourIndex));

			addedToList = true;
			break;
		}
	}

	if (!addedToList && scoreData.size() < 10)
	{
		if(mPlayerName != "")
			scoreData.push_back(ScoreData(mCurrentScore, mPlayerName, mPlayerNameColourIndex));
		else
			scoreData.push_back(ScoreData(mCurrentScore, "UNNAMED", mPlayerNameColourIndex));
	}

	// Cap scores count to 10
	while (scoreData.size() > 10)
		scoreData.pop_back();

	// Save out the scores to the file
	ofstream writeFile("Save Data/HighScores.txt", std::ofstream::trunc);

	if (!writeFile.is_open())
	{
		std::cout << "Failed to write to the highscores file" << std::endl;
		return;
	}

	for (unsigned int i = 0; i < scoreData.size(); i++)
		writeFile << scoreData[i].name << " " << scoreData[i].score << " " << scoreData[i].colourIndex << "\n";

	writeFile.close();

	delete[] cLine;
}

// ---------------------------------------------------------------- //

unsigned int GameManager::LoadInCurrentHighScore()
{
	// First open the high scores file
	ifstream readFile("Save Data/HighScores.txt");

	if (!readFile.is_open())
	{
		std::cout << "Failed to load in the current highscore from the save data." << std::endl;
		return 0;
	}

	// We just need to load in the first line of the file and get the score from it
	char* cLine               = new char[100];
	unsigned int              returnVal;
	std::string               sLine, temp;
	std::stringstream         ssLine;

	readFile.getline(cLine, 100);

	sLine = cLine;
	ssLine = std::stringstream(sLine);

	ssLine >> temp >> returnVal;

	// Clean up the memory allocated
	delete[] cLine;

	return returnVal;
}

// ---------------------------------------------------------------- //

void GameManager::IncrementGhostsEatenCount()
{
	mGhostsEatenThisGame++; 
	mGhostsEatenStreak++;

	switch (mGhostsEatenStreak)
	{
	case 1:
		AddToScore(100);
	break;

	case 2:
		AddToScore(300);
	break;

	case 3:
		AddToScore(700);
	break;

	case 4:
		AddToScore(1000);
	break;

	default:
		AddToScore(1000);
	break;
	}
}
