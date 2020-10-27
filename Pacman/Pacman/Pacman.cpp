#include "Pacman.h"
#include "GameManager.h"
#include "Constants.h"

#include "UIManager.h"

#include <sstream>

// -------------------------------------------------------------------------------------------------------------- //

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	_frameCount = 0;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, SCREEN_WIDTH, SCREEN_HEIGHT, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

// -------------------------------------------------------------------------------------------------------------- //

Pacman::~Pacman()
{
	delete mBackground;
	delete mPlayer;
	delete mDotHandler;

	delete mPauseMenu;
	delete mStartMenu;
	delete mHighScoreMenu;

	delete mCollectable;
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::LoadContent()
{
	mBackground      = new Background(14, 3);

	mPlayer          = new PacmanCharacter(mBackground->GetCollisionMap());
	mDotHandler      = new DotsHandler();

	mPauseMenu       = new PauseMenu();
	mStartMenu       = new StartMenu();
	mHighScoreMenu   = new HighScoresMenu();

	// Default into the start menu
	mInStartMenu     = true;
	mInHighscoreMenu = false;
	mInMainGame      = false;

	mTimeTillNextCollectableSpawn = 10.0f;
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::Update(int elapsedTime)
{
	float deltaTime = ((float)elapsedTime / 1000.0f);

	if (mInStartMenu)
		StartMenuUpdate(deltaTime);

	if (mInHighscoreMenu)
		HighScoreMenuUpdate();

	if(GameManager::Instance()->GetGameIsPaused())
		PauseMenuUpdate(deltaTime);

	if(mInMainGame)
		InGameUpdate(deltaTime);
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::Draw(int elapsedTime)
{
	// Starts Drawing
	SpriteBatch::BeginDraw();

		// Make sure we are rendering the correct content
		if (mInMainGame)
		{
			MainGameRender();
		}
		else
		{
			if (mInStartMenu)
			{
				mStartMenu->Render(_frameCount);
			}
			else if (mInHighscoreMenu)
			{
				mHighScoreMenu->Render();
			}
			else if (GameManager::Instance()->GetGameIsPaused())
			{
				mPauseMenu->Render();
			}
		}

		// Increment the frame count
		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;

	SpriteBatch::EndDraw(); // Ends Drawing
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::StartMenuUpdate(const float deltaTime)
{
	mInMainGame = false;

	SELECTION_OPTIONS returnOption = mStartMenu->Update(deltaTime);
	if (returnOption == SELECTION_OPTIONS::START_GAME)
	{
		mInMainGame      = true;
		mInStartMenu     = false;
		mInHighscoreMenu = false;
	}
	else if (returnOption == SELECTION_OPTIONS::HIGHSCORES)
	{
		// Selected to go into the highscores menu
		mInMainGame      = false;
		mInStartMenu     = false;
		mInHighscoreMenu = true;
	}
	else if (returnOption == SELECTION_OPTIONS::CHANGE_PLAYER)
	{
		GameManager::Instance()->IncrementPlayerCharacter();
		mInMainGame = false;
	}
	else if (returnOption == SELECTION_OPTIONS::QUIT)
	{
		std::cout << "Quitting the program." << std::endl;
	}
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::PauseMenuUpdate(const float deltaTime)
{
	if (mPauseMenu)
	{
		mInMainGame = mPauseMenu->Update(deltaTime);
	}
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::HighScoreMenuUpdate()
{
	mInMainGame = false;

	if (!mHighScoreMenu->Update())
	{
		mInHighscoreMenu = false;
		mInStartMenu = true;
	}
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::InGameUpdate(const float deltaTime)
{
	// Collectable update
	mTimeTillNextCollectableSpawn -= deltaTime;

	if (mTimeTillNextCollectableSpawn <= 0.0f && mCollectable == nullptr)
	{
		// Spawn the collectable
		SpawnNextCollectable();

		// Reset the time to a random amount
		mTimeTillNextCollectableSpawn = float((rand() % 40) + 15);
	}

	// Player update
	mPlayer->Update(deltaTime);

	// Background Update
	mBackground->Update();

	// Update the dots in the level
	if (mDotHandler->Update(mPlayer->GetCentrePosition(), 9))
	{
		mPlayer->ResetCharacter();
		return;
	}

	// Update the game manager
	GameManager::Instance()->Update(deltaTime);

	// Collectable collision
	if (mCollectable && mCollectable->CheckForCollision(mPlayer->GetCentrePosition(), 8, mPlayer->GetFacingDirection()))
	{
		// Add the relevent score
		GameManager::Instance()->AddToScore((int)mCollectable->GetType() * 100);

		// Delete this collectable
		delete mCollectable;
		mCollectable = nullptr;
	}

	// Input
	InGameInputCheck();
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::InGameInputCheck()
{
	// Check if the player has paused the game
	if (S2D::Input::Keyboard::GetState()->IsKeyDown(S2D::Input::Keys::P) && !GameManager::Instance()->GetIsAlreadySettingGamePaused())
	{
		GameManager::Instance()->SetGameIsPaused(true);
		GameManager::Instance()->SetIsPausedButtonPressed(true);
		return;
	}
	else if (S2D::Input::Keyboard::GetState()->IsKeyUp(S2D::Input::Keys::P))
	{
		GameManager::Instance()->SetIsPausedButtonPressed(false);
	}

	// If we want to return back to the start menu then the player must press escape
	if (S2D::Input::Keyboard::GetState()->IsKeyDown(S2D::Input::Keys::ESCAPE))
	{
		mInStartMenu = true;
		return;
	}
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::MainGameRender()
{
	// Draw the background first
	if (mBackground)
		mBackground->Render();

	// Render the dots onto the screen
	if (mDotHandler)
		mDotHandler->Render(_frameCount);

	if (mCollectable)
		mCollectable->Render();

	// Render the player
	if (mPlayer)
		mPlayer->Render(_frameCount);

	UIManager::GetInstance()->Render();
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::SpawnNextCollectable()
{
	// Spawn in a new random collectable
	mCollectable = new PickUps((PICKUP_TYPES) (rand() % 8));
}

// -------------------------------------------------------------------------------------------------------------- //