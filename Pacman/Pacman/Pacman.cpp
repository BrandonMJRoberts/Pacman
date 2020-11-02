#include "Pacman.h"
#include "GameManager.h"
#include "Constants.h"

#include "UIManager.h"
#include "AudioManager.h"

#include <time.h>

#include <sstream>

// -------------------------------------------------------------------------------------------------------------- //

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	// Set the seed to be random
	srand(time(NULL));

	_frameCount = 0;

	//Initialise important Game aspects
	S2D::Audio::Initialise();
	Graphics::Initialise(argc, argv, this, SCREEN_WIDTH, SCREEN_HEIGHT, false, 25, 25, "Pacman", FRAME_RATE);
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
	delete mTextRenderer;

	Graphics::Destroy();
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::LoadContent()
{
	if(!mStartMenu)
		mStartMenu		  = new StartMenu();

	// Default into the start menu
	mInStartMenu     = true;
	mInHighscoreMenu = false;
	mInMainGame      = false;

	if(!mTextRenderer)
		mTextRenderer = new TextRenderer("Textures/UI/Font.png", 15, 21);
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::Update(int elapsedTime)
{
	float deltaTime = ((float)elapsedTime / 1000.0f);

	if (mInStartMenu)
		StartMenuUpdate(deltaTime);
	else if (mInHighscoreMenu)
		HighScoreMenuUpdate();
	else if(GameManager::Instance()->GetGameIsPaused())
		PauseMenuUpdate(deltaTime);
	else if(mInMainGame)
		InGameUpdate(deltaTime);
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::Draw(int elapsedTime)
{
	// Starts Drawing
	SpriteBatch::BeginDraw();

		// Make sure we are rendering the correct content
		if (mInMainGame)
			MainGameRender();
		else if (mInStartMenu)
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

		// Increment the frame count
		_frameCount++;

		if (_frameCount >= FRAME_RATE)
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

		LoadInDataForLevel();
	}
	else if (returnOption == SELECTION_OPTIONS::HIGHSCORES)
	{
		// Selected to go into the highscores menu
		mInMainGame      = false;
		mInStartMenu     = false;
		mInHighscoreMenu = true;

		if (!mHighScoreMenu)
			mHighScoreMenu = new HighScoresMenu();
	}
	else if (returnOption == SELECTION_OPTIONS::CHANGE_PLAYER)
	{
		GameManager::Instance()->IncrementPlayerCharacter();
		mInMainGame      = false;
		mInHighscoreMenu = false;
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
	// Update the dots in the level
	mDotHandler->Update(mPlayer->GetCentrePosition(), 9);

    // First check if the level is over
	if (GameManager::Instance()->GetRemainingDots() == 0)
	{
		GameManager::Instance()->LoadLevel(GameManager::Instance()->GetCurrentLevel() + 1);
		mPlayer->ResetCharacter();

		// Make sure we change the background colour to being the next level's
		mBackground->ChangeColourIndex(GameManager::Instance()->GetCurrentLevel());

		delete mCollectable;
		mCollectable = nullptr;

		return;
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

	UIManager::GetInstance()->Render();

	// Render the dots onto the screen
	if (mDotHandler)
		mDotHandler->Render(_frameCount);

	if (mCollectable)
		mCollectable->Render();

	// Render the player
	if (mPlayer)
		mPlayer->Render(_frameCount);
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::SpawnNextCollectable()
{
	// Spawn in a new random collectable
	if(!mCollectable)
		mCollectable = new PickUps(GameManager::Instance()->GetThisLevelCollectableType());
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::LoadInDataForLevel()
{
	if(!mBackground)
		mBackground  = new Background(14, 4);

	if(!mPauseMenu)
		mPlayer      = new PacmanCharacter(mBackground->GetCollisionMap());

	if(!mDotHandler)
		mDotHandler  = new DotsHandler();

	if(!mPauseMenu)
		mPauseMenu   = new PauseMenu();

	mTimeTillNextCollectableSpawn = 10.0f;
}

// -------------------------------------------------------------------------------------------------------------- //