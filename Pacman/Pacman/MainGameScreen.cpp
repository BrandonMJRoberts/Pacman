#include "MainGameScreen.h"

#include "GameManager.h"
#include "UIManager.h"

#include "PacmanCharacter.h"
#include "Ghost.h"

#include "Pickups.h"
#include "Background.h"
#include "DotsHandler.h"
#include "AudioManager.h"

// ------------------------------------------------------------------------------ //

MainGameScreen::MainGameScreen() : BaseMenu()
{
	LoadInDataForLevel();

	GameManager::Instance()->ResetPreGameTimer();

	UIManager::GetInstance()->AddCollectedPickup(GameManager::Instance()->GetThisLevelCollectableType());
	UIManager::GetInstance()->ResetExtraLifeSprite();
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

	if(mPacman)
		mPacman->Render(frameCount);

	// Render all of the ghosts
	for (unsigned int i = 0; i < mGhosts.size(); i++)
	{
		if (mGhosts[i])
			mGhosts[i]->Render(frameCount);
	}
}

// ------------------------------------------------------------------------------ //

SCREENS MainGameScreen::Update(const float deltaTime)
{
	if (!GameManager::Instance()->GetIsInPreGameState())
	{
		// Update the dots in the level
		if (mPacman)
		{
			mPacman->Update(deltaTime);
			mDotHandler->Update(mPacman->GetCentrePosition(), 0.5f);
		}

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

		// Update all ghosts
		for (unsigned int i = 0; i < mGhosts.size(); i++)
		{
			if (mGhosts[i])
				mGhosts[i]->Update(deltaTime, mPacman->GetCentrePosition(), mPacman->GetFacingDirection());
		}

		HandleCollectable(deltaTime);

		// Now check if pacman has collided with any of the ghosts
		CheckForCharacterCollisions();

		// Now check if the game is over due to death counter
		if (GameManager::Instance()->GetExtraLivesCount() <= 0)
			return SCREENS::MAIN_MENU;

		// Update the game manager
		UIManager::GetInstance()->Update(deltaTime);
	}

	GameManager::Instance()->Update(deltaTime);

	// Input
	return InGameInputCheck();
}

// ------------------------------------------------------------------------------ //

void MainGameScreen::LoadInDataForLevel()
{
	// Load in the background
	if (!mBackground)
		mBackground = new Background(14, 4);

	// Create the dots handler
	if (!mDotHandler)
		mDotHandler = new DotsHandler();

	// Load in pacman
	if (!mPacman)
	{
		if (GameManager::Instance()->GetPlayerCharacterType() == PLAYER_CHARACTER_TYPE::PACMAN)
			mPacman = new PacmanCharacter(mBackground->GetCollisionMap(), 3, 3, 3, 4, S2D::Vector2(14.0f, 23.5f), "Textures/Pacman/PacmanSprites.png", "Textures/Pacman/PacmanDeathAnimation.png", false);
		else
			mPacman = new PacmanCharacter(mBackground->GetCollisionMap(), 3, 3, 3, 4, S2D::Vector2(14.0f, 23.5f), "Textures/Pacman/PacmanSprites.png", "Textures/Pacman/PacmanDeathAnimation.png", true);
	}

	if (mGhosts.size() == 0)
	{
		for (unsigned int i = 0; i < NUMBER_OF_GHOSTS_IN_LEVEL; i++)
		{
			if (GameManager::Instance()->GetPlayerCharacterType() == (PLAYER_CHARACTER_TYPE)(i + 1))
				mGhosts.push_back(new Ghost(S2D::Vector2(1.5f, 1.5f), mBackground->GetCollisionMap(), (GHOST_TYPE)i, false, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2));
			else
				mGhosts.push_back(new Ghost(S2D::Vector2(1.5f, 1.5f), mBackground->GetCollisionMap(), (GHOST_TYPE)i, true, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2));
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

		AudioManager::GetInstance()->StopAllAudio();

		return SCREENS::MAIN_MENU;
	}

	return SCREENS::SAME;
}

// ------------------------------------------------------------------------------ //

void MainGameScreen::HandleCollectable(const float deltaTime)
{
	// Collectable collision
	if (mCollectable && mCollectable->CheckForCollision(mPacman->GetCentrePosition(), 0.8f, mPacman->GetFacingDirection()))
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
			mTimeTillNextCollectableSpawn = float((rand() % 40) + 15);
		}
	}
}

// ------------------------------------------------------------------------------ //

void MainGameScreen::CheckForCharacterCollisions()
{
	if (mPacman)
	{
		S2D::Vector2 ghostCentre, pacmanCentre = mPacman->GetCentrePosition();
		float accuracy = 0.5f;

		// Loop through all of the ghosts and check for collisions
		for (unsigned int i = 0; i < mGhosts.size(); i++)
		{
			if (mGhosts[i] && !mGhosts[i]->GetIfGhostIsEaten())
			{
				ghostCentre = mGhosts[i]->GetCentrePosition();

				if ((ghostCentre.X == pacmanCentre.X && ghostCentre.Y + accuracy > pacmanCentre.Y - accuracy && ghostCentre.Y - accuracy < pacmanCentre.Y + accuracy) ||
					(ghostCentre.Y == pacmanCentre.Y && ghostCentre.X + accuracy > pacmanCentre.X - accuracy && ghostCentre.X - accuracy < pacmanCentre.X + accuracy))
				{
					GameManager* GM = GameManager::Instance();

					if (GM->GetIsPlayerPoweredUp())
					{
						if (mGhosts[i])
						{
							if (mGhosts[i]->IsAlive() && GameManager::Instance()->GetPlayerCharacterType() != PLAYER_CHARACTER_TYPE::PACMAN)
							{
								// Remove the life if we are playing as a ghost
								GameManager::Instance()->RemoveLife();
							}
							else
							{
								// Increase the amounts of ghost eaten this game
								GM->IncrementGhostsEatenCount();
							}

							// Set this ghost as being dead and eaten
							mGhosts[i]->SetGhostIsEaten(true);
							mGhosts[i]->SetIsAlive(false);

							// Display the points
							UIManager::GetInstance()->DisplayPoints(mPacman->GetCentrePosition(), true, GM->GetAmountOfGhostsEatenStreak() - 1);

							// Play the correct audio track
							AudioManager::GetInstance()->PlayEatingGhostSFX();

							// Also play the sfx for the ghosts going to their home
							AudioManager::GetInstance()->PlayGhostGoingToHomeSFX();
						}
					}
					else
					{
						if (mPacman)
						{
							if (mPacman->IsAlive() && GameManager::Instance()->GetPlayerCharacterType() == PLAYER_CHARACTER_TYPE::PACMAN)
							{
								// Remove a life
								GameManager::Instance()->RemoveLife();

								// Increase the amount of pacman kills the player has currently got this game
								GameManager::Instance()->IncreasePacmanDeathCounter();

							}

							mPacman->SetIsAlive(false);

							if (GameManager::Instance()->GetExtraLivesCount() > 1)
								AudioManager::GetInstance()->PlayPacmanDeathSFX_1();
							else
								AudioManager::GetInstance()->PlayPacmanDeathSFX_2();
						}
					}
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------ //