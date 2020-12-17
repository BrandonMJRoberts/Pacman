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

	// Default to one ghost being released and the time per ghost release
	mAmountOfGhostsReleased       = 1;
	mTimeRemainingForGhostRelease = TIME_PER_GHOST_RELEASE;

	GameManager::Instance()->ResetPreGameTimer();
	GameManager::Instance()->ResetScoreForExtraLife(); // Make sure that the amount of points required for the next life are correctly set

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

	UIManager::GetInstance()->RemoveAllCollectedPickups();
	
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

	// Render the dots onto the screen
	if (mDotHandler)
		mDotHandler->Render(frameCount);

	if (mCollectable)
		mCollectable->Render();

	UIManager::GetInstance()->Render();

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
	AudioManager::GetInstance()->Update();

	if (!GameManager::Instance()->GetIsInPreGameState())
	{
		// Update the dots in the level
		if (mPacman)
		{
			// Get all of the ghosts positions to pass to pacmans
			std::vector<S2D::Vector2> ghostPositions;
			for (unsigned int i = 0; i < mGhosts.size(); i++)
			{
				if (mGhosts[i])
					ghostPositions.push_back(mGhosts[i]->GetCentrePosition());
			}

			mPacman->Update(deltaTime, ghostPositions, *mDotHandler);

			if(mDotHandler)
				mDotHandler->Update(mPacman->GetCentrePosition(), 0.5f);
		}

		// First check if the level is over
		if (GameManager::Instance()->GetRemainingDots() == 0)
		{
			LoadNextLevel();
		}

		// Update all ghosts
		for (unsigned int i = 0; i < mGhosts.size(); i++)
		{
			if (mGhosts[i])
				mGhosts[i]->Update(deltaTime, mPacman->GetCentrePosition(), mPacman->GetFacingDirection());
		}

		// Remove the time passed for a ghost release
		mTimeRemainingForGhostRelease -= deltaTime;

		// Check if enough time has passed for another ghost to be released
		if (mAmountOfGhostsReleased < NUMBER_OF_GHOSTS_IN_LEVEL && mTimeRemainingForGhostRelease < 0.0f)
		{
			// Then release a ghost

			// Loop through all the ghosts and release the next one that has not been released
			for (unsigned int i = 0; i < mGhosts.size(); i++)
			{
				if (!mGhosts[i]->GetCanLeaveHome())
				{
					mGhosts[i]->SetCanLeaveHome(true);
					mAmountOfGhostsReleased++;
					break;
				}
			}

			// Reset the time remaining
			mTimeRemainingForGhostRelease = TIME_PER_GHOST_RELEASE;
		}

		HandleCollectable(deltaTime);

		// Now check if pacman has collided with any of the ghosts
		CheckForCharacterCollisions();

		// Now check if the game is over due to death counter
		if (GameManager::Instance()->GetExtraLivesCount() < 0)
		{
			UIManager::GetInstance()->ResetForStartOfGame();

			mAmountOfGhostsReleased       = 1;
			mTimeRemainingForGhostRelease = TIME_PER_GHOST_RELEASE;

			// Save the current score to the highscore table
			GameManager::Instance()->SaveOutScoreToLeaderboard();

			// Reset the current score to zero
			GameManager::Instance()->SetCurrentScore(0);

			return SCREENS::MAIN_MENU;
		}

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
			mPacman = new PacmanCharacter(mBackground->GetCollisionMap(), 3, 3, 3, 5, S2D::Vector2(14.0f, 23.5f), "Textures/Pacman/PacmanSprites.png", "Textures/Pacman/PacmanDeathAnimation.png", false);
		else
			mPacman = new PacmanCharacter(mBackground->GetCollisionMap(), 3, 3, 3, 5, S2D::Vector2(14.0f, 23.5f), "Textures/Pacman/PacmanSprites.png", "Textures/Pacman/PacmanDeathAnimation.png", true);
	}

	// Create the ghosts 
	if (mGhosts.size() == 0)
	{
		S2D::Vector2 outSideHomeSpawn = S2D::Vector2(14.0f, 11.5f);

		// First make sure that the player always spawns as the first ghost in the maze
		if (GameManager::Instance()->GetPlayerCharacterType() != PLAYER_CHARACTER_TYPE::PACMAN)
		{
			switch (GameManager::Instance()->GetPlayerCharacterType())
			{
			case PLAYER_CHARACTER_TYPE::RED_GHOST:
				mGhosts.push_back(new Ghost(outSideHomeSpawn, mBackground->GetCollisionMap(), GHOST_TYPE::RED, false, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2, false));
			break;

			case PLAYER_CHARACTER_TYPE::BLUE_GHOST:
				mGhosts.push_back(new Ghost(outSideHomeSpawn, mBackground->GetCollisionMap(), GHOST_TYPE::BLUE, false, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2, false));
			break;

			case PLAYER_CHARACTER_TYPE::ORANGE_GHOST:
				mGhosts.push_back(new Ghost(outSideHomeSpawn, mBackground->GetCollisionMap(), GHOST_TYPE::ORANGE, false, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2, false));
			break;

			case PLAYER_CHARACTER_TYPE::PINK_GHOST:
				mGhosts.push_back(new Ghost(outSideHomeSpawn, mBackground->GetCollisionMap(), GHOST_TYPE::PINK, false, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2, false));
			break;

			default: break;
			}

			// Now create the remaining ghosts inside of the home
			unsigned int GhostSpawnID = 0;
			for (unsigned int i = 0; i < NUMBER_OF_GHOSTS_IN_LEVEL; i++)
			{
				if ((PLAYER_CHARACTER_TYPE)(i + 1) == GameManager::Instance()->GetPlayerCharacterType())
					continue;
				else
				{
					mGhosts.push_back(new Ghost(S2D::Vector2(12.0f + (GhostSpawnID * 2.0f), 14.5f), mBackground->GetCollisionMap(), (GHOST_TYPE)(i), true, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2, true));
					GhostSpawnID++;
				}
			}
		}
		else
		{
			// Just create the ghosts normally - with the red ghost on the outside first
			mGhosts.push_back(new Ghost(outSideHomeSpawn,           mBackground->GetCollisionMap(), GHOST_TYPE::RED,    true, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2, false));
			mGhosts.push_back(new Ghost(S2D::Vector2(12.0f, 14.5f), mBackground->GetCollisionMap(), GHOST_TYPE::BLUE,   true, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2, true));
			mGhosts.push_back(new Ghost(S2D::Vector2(14.0f, 14.5f), mBackground->GetCollisionMap(), GHOST_TYPE::ORANGE, true, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2, true));
			mGhosts.push_back(new Ghost(S2D::Vector2(16.0f, 14.5f), mBackground->GetCollisionMap(), GHOST_TYPE::PINK,   true, "Textures/Ghosts/Ghosts.png", "Textures/Ghosts/SpecialStates.png", 8, 4, 4, 2, true));
		}
	}

	mTimeTillNextCollectableSpawn = 10.0f;
	mCollectable                  = nullptr;
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

		AudioManager::GetInstance()->PauseAllAudio();

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
		gm->ResetGhostsEatenStreak();                      // Reset the ghosts eaten streak
		gm->ResetAmountOfGhostsReleased();
		 
		mTimeRemainingForGhostRelease = TIME_PER_GHOST_RELEASE;
		mAmountOfGhostsReleased       = 1;

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
	// Only do this if pacman has been created
	if (mPacman)
	{
		S2D::Vector2 ghostCentre, pacmanCentre = mPacman->GetCentrePosition();
		float        accuracy = 0.5f;

		// Loop through all of the ghosts and check for collisions
		for (unsigned int i = 0; i < mGhosts.size(); i++)
		{
			// If the ghost exists and the ghost is not currently eaten
			if (mGhosts[i] && !mGhosts[i]->GetIfGhostIsEaten())
			{
				// Get the ghost position
				ghostCentre = mGhosts[i]->GetCentrePosition();

				// Check for a collision between this ghost and pacman
				if ((ghostCentre.X == pacmanCentre.X && ghostCentre.Y + accuracy > pacmanCentre.Y - accuracy && ghostCentre.Y - accuracy < pacmanCentre.Y + accuracy) ||
					(ghostCentre.Y == pacmanCentre.Y && ghostCentre.X + accuracy > pacmanCentre.X - accuracy && ghostCentre.X - accuracy < pacmanCentre.X + accuracy))
				{
					// Gamemanager instance getting once and storing it, so we dont have to do it multiple times
					GameManager* GM = GameManager::Instance();

					// If the player is powered up
					if (GM->GetIsPlayerPoweredUp())
					{
						// If there is a collision between this ghost and pacman, and the player is not playing as pacman, and this ghost is alive
						if (mGhosts[i]->IsAlive() && GameManager::Instance()->GetPlayerCharacterType() != PLAYER_CHARACTER_TYPE::PACMAN)
						{
							// Then remove a life if we are playing as a ghost
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
						UIManager::GetInstance()->DisplayPoints((mPacman->GetCentrePosition() * SPRITE_RESOLUTION) + GameManager::Instance()->GetGridOffset(), true, GM->GetAmountOfGhostsEatenStreak() - 1);

						// Play the correct audio track
						AudioManager::GetInstance()->PlayEatingGhostSFX();

						return;
					}
					else
					{
						if (mPacman->IsAlive())
						{
							GameManager* gm = GameManager::Instance();

							if (gm->GetPlayerCharacterType() == PLAYER_CHARACTER_TYPE::PACMAN)
							{
								// Remove a life
								gm->RemoveLife();

								// Increase the amount of pacman kills the player has currently got this game
								gm->IncreasePacmanDeathCounter();

								// Reset the needed game mamager variables
								gm->ResetGhostsEatenStreak();                      // Reset the ghosts eaten streak
								gm->ResetAmountOfGhostsReleased();

								mTimeRemainingForGhostRelease = TIME_PER_GHOST_RELEASE;
								mAmountOfGhostsReleased       = 1;
							}
							else
							{
								// Make sure we add the needed points to the current score
								gm->AddToScore(POINTS_PER_PACMAN_KILL / GameManager::Instance()->GetAmountOfGhostsReleased());
							}
						}

						// Kill pacman
						mPacman->SetIsAlive(false);

						// We also need to make sure that the ghosts are reset when pacman dies
						for (unsigned int i = 0; i < mGhosts.size(); i++)
						{
							if(mGhosts[i])
								mGhosts[i]->SetGhostsShouldReset();
						}

						// Play the pacman death SFX 
						AudioManager::GetInstance()->PlayPacmanDeathSFX_1();

						return;
					}
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------ //

void MainGameScreen::LoadNextLevel()
{
	// Increase the level in the game manager
	GameManager::Instance()->LoadLevel(GameManager::Instance()->GetCurrentLevel() + 1);

	// Reset the player
	mPacman->ResetCharacter();

	// Make sure we change the background colour to being the next level's
	mBackground->ChangeColourIndex(GameManager::Instance()->GetCurrentLevel());

	// Make sure the ghosts reset as well as the rest of the level
	for (unsigned int i = 0; i < mGhosts.size(); i++)
		mGhosts[i]->SetGhostsShouldReset();

	mTimeRemainingForGhostRelease = TIME_PER_GHOST_RELEASE;
	mAmountOfGhostsReleased = 1;

	AudioManager::GetInstance()->StopAllAudio();

	delete mCollectable;
	mCollectable = nullptr;
}

// ------------------------------------------------------------------------------ //