#include "MainGameScreen.h"

#include "GameManager.h"
#include "UIManager.h"

#include "PacmanCharacter.h"
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

	delete mPlayer;
	mPlayer = nullptr;

	delete mDotHandler;
	mDotHandler = nullptr;

	UIManager::GetInstance()->RemoveALlCollectedPickups();
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

	// Render the player
	if (mPlayer)
		mPlayer->Render(frameCount);
}

// ------------------------------------------------------------------------------ //

SCREENS MainGameScreen::Update(const float deltaTime)
{
	// Update the dots in the level
	mDotHandler->Update(mPlayer->GetCentrePosition(), 9);

	// First check if the level is over
	if (GameManager::Instance()->GetRemainingDots() == 0)
	{
		// Increase the level in the game manager
		GameManager::Instance()->LoadLevel(GameManager::Instance()->GetCurrentLevel() + 1);

		// Reset the player
		mPlayer->ResetCharacter();

		// Make sure we change the background colour to being the next level's
		mBackground->ChangeColourIndex(GameManager::Instance()->GetCurrentLevel());

		delete mCollectable;
		mCollectable = nullptr;

		// Now need to refresh the game
	}

	// Player update
	mPlayer->Update(deltaTime);

	// Collectable collision
	if (mCollectable && mCollectable->CheckForCollision(mPlayer->GetCentrePosition(), 13, mPlayer->GetFacingDirection()))
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

	if (!mPlayer)
		mPlayer = new PacmanCharacter(mBackground->GetCollisionMap(), 3, 3);

	if (!mDotHandler)
		mDotHandler = new DotsHandler();

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
		GameManager::Instance()->SetCurrentLevel(0);                            // Reset the level ID
		GameManager::Instance()->SetCurrentScore(0);                            // Reset the player's current score
		GameManager::Instance()->SetLevelPickupType(PICKUP_TYPES::CHERRY);      // Set the starting collectable type
		GameManager::Instance()->SetCurrentExtraLifeCount(STARTING_LIFE_COUNT); // Reset the starting life count


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