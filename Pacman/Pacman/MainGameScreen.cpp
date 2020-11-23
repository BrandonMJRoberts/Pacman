#include "MainGameScreen.h"

#include "GameManager.h"
#include "UIManager.h"

#include "PacmanCharacter.h"
#include "Ghost.h"

#include "Pickups.h"
#include "Background.h"
#include "DotsHandler.h"

// ------------------------------------------------------------------------------ //

MainGameScreen::MainGameScreen() : BaseMenu()
{
	LoadInDataForLevel();

	UIManager::GetInstance()->AddCollectedPickup(GameManager::Instance()->GetThisLevelCollectableType());
}

// ------------------------------------------------------------------------------ //

MainGameScreen::~MainGameScreen()
{
	if (mCollectable)
	{
		delete mCollectable;
		mCollectable = nullptr;
	}

	delete mBackground;
	mBackground = nullptr;

	delete mDotHandler;
	mDotHandler = nullptr;

	UIManager::GetInstance()->RemoveALlCollectedPickups();

	delete mPacman;
	mPacman = nullptr;

	for (unsigned int i = 0; i < mGhosts.size(); i++)
	{
		delete mGhosts[i];
		mGhosts[i] = nullptr;
	}
	mGhosts.clear();
}

// ------------------------------------------------------------------------------ //

void MainGameScreen::Render(const unsigned int frameCount)
{
	// Draw the background first
	if (mBackground)
		mBackground->Render();

	UIManager::GetInstance()->Render();

	// Render the dots onto the screen
	if (mDotHandler)
		mDotHandler->Render(frameCount);

	if (mCollectable)
		mCollectable->Render();

	if (mPacman)
		mPacman->Render(frameCount);

	for (unsigned int i = 0; i < mGhosts.size(); i++)
	{
		if (mGhosts[i])
			mGhosts[i]->Render(frameCount);
	}
}

// ------------------------------------------------------------------------------ //

SCREENS MainGameScreen::Update(const float deltaTime)
{
	// Update the dots in the level
	mDotHandler->Update(mPacman->GetCentrePosition(), 9);

	// First check if the level is over
	if (GameManager::Instance()->GetRemainingDots() == 0)
	{
		// Increase the level in the game manager
		GameManager::Instance()->LoadLevel(GameManager::Instance()->GetCurrentLevel() + 1);

		// Reset the player
		mPacman->ResetCharacter();

		// Make sure we change the background colour to being the next level's
		mBackground->ChangeColourIndex(GameManager::Instance()->GetCurrentLevel());

		delete mCollectable;
		mCollectable = nullptr;
	}

	// Ghosts update
	for (unsigned int i = 0; i < mGhosts.size(); i++)
	{
		if (mGhosts[i])
			mGhosts[i]->Update(deltaTime, mPacman->GetCentrePosition(), mPacman->GetFacingDirection());
	}

	// Pacman update
	if(mPacman)
		mPacman->Update(deltaTime);

	HandleCollectable(deltaTime);

	// Update the game manager
	GameManager::Instance()->Update(deltaTime);
	UIManager::GetInstance()->Update(deltaTime);

	// Input
	return InGameInputCheck();
}

// ------------------------------------------------------------------------------ //

void MainGameScreen::LoadInDataForLevel()
{
	if (!mBackground)
		mBackground = new Background(14, 4);

	if (!mDotHandler)
		mDotHandler = new DotsHandler();

	if (!mPacman)
	{
		if(GameManager::Instance()->GetPlayerCharacterType() == PLAYER_CHARACTER_TYPE::PACMAN)
			mPacman = new PacmanCharacter(mBackground->GetCollisionMap(), 3, 3, false);
		else
			mPacman = new PacmanCharacter(mBackground->GetCollisionMap(), 3, 3, true);
	}

	if (mGhosts.size() == 0)
	{
		for (unsigned int i = 0; i < NUMBER_OF_GHOSTS_IN_LEVEL; i++)
		{
			if (((int)GameManager::Instance()->GetPlayerCharacterType()) - 1 == i)
				mGhosts.push_back(new Ghost(S2D::Vector2(1.0f, 1.0), mBackground->GetCollisionMap(), (GHOST_TYPE)i, false, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4));
			else
				mGhosts.push_back(new Ghost(S2D::Vector2(1.0f, 1.0), mBackground->GetCollisionMap(), (GHOST_TYPE)i, true, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4));
		}
	}

	mTimeTillNextCollectableSpawn = 10.0f;

	mCollectable = nullptr;
}

// ------------------------------------------------------------------------------ //

void MainGameScreen::SpawnNextCollectable()
{
	// Spawn in a new random collectable
	if (!mCollectable)
		mCollectable = new PickUps(GameManager::Instance()->GetThisLevelCollectableType());
}

// ------------------------------------------------------------------------------ //

SCREENS MainGameScreen::InGameInputCheck()
{
	// Check if the player has paused the game
	if (S2D::Input::Keyboard::GetState()->IsKeyDown(S2D::Input::Keys::P) && !GameManager::Instance()->GetIsAlreadySettingGamePaused())
	{
		GameManager::Instance()->SetGameIsPaused(true);
		GameManager::Instance()->SetIsPausedButtonPressed(true);

		return SCREENS::PAUSE_MENU;
	}
	else if (S2D::Input::Keyboard::GetState()->IsKeyUp(S2D::Input::Keys::P))
	{
		GameManager::Instance()->SetIsPausedButtonPressed(false);
	}

	// If we want to return back to the start menu then the player must press escape
	if (S2D::Input::Keyboard::GetState()->IsKeyDown(S2D::Input::Keys::ESCAPE))
	{
		GameManager* gm = GameManager::Instance();

		gm->SetCurrentLevel(0);                            // Reset the level ID
		gm->SetLevelPickupType(PICKUP_TYPES::CHERRY);      // Set the starting collectable type
		gm->SetCurrentExtraLifeCount(STARTING_LIFE_COUNT); // Reset the starting life count
		gm->ResetScoreForExtraLife();                      // Reset the points required for another life
		gm->SaveOutScoreToLeaderboard();                   // Save the player's score out
		gm->SetCurrentScore(0);                            // Reset the player's current score
		gm->ResetDotsEatenCount();                         // Reset the dots eaten count
		gm->ResetGhostsEatenCount();					   // Reset the ghosts eaten count

		return SCREENS::MAIN_MENU;
	}

	return SCREENS::SAME;
}

// ------------------------------------------------------------------------------ //

/*
void MainGameScreen::LoadNextLevel()
{
	delete mBackground;
	mBackground = nullptr;

	delete mPlayer;
	mPlayer = nullptr;

	delete mDotHandler;
	mDotHandler = nullptr;

	LoadInDataForLevel();
}
*/
// ------------------------------------------------------------------------------ //

void MainGameScreen::HandleCollectable(const float deltaTime)
{
	// Collectable collision
	if (mCollectable && mCollectable->CheckForCollision(mPacman->GetCentrePosition(), 13, mPacman->GetFacingDirection()))
	{
		// Delete this collectable
		delete mCollectable;
		mCollectable = nullptr;
	}

	if (mCollectable == nullptr)
	{
		// Collectable update
		mTimeTillNextCollectableSpawn -= deltaTime;

		if (mTimeTillNextCollectableSpawn <= 0.0f)
		{
			// Spawn the collectable
			SpawnNextCollectable();

			// Reset the time to a random amount
			mTimeTillNextCollectableSpawn = 5; //= float((rand() % 40) + 15);
		}
	}
}