#include "Pacman.h"
#include "Constants.h"

#include "StartMenu.h"
#include "PauseMenu.h"
#include "HighScoresMenu.h"
#include "MainGameScreen.h"

#include <time.h>

#include <sstream>

// -------------------------------------------------------------------------------------------------------------- //

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv)
{
	// Set the seed to be a random one
	srand((unsigned int)time(NULL));

	mFrameCount = 0;

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
	delete mCurrentScreen;
	delete mGameInstance;

	Graphics::Destroy();
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::LoadContent()
{
	// Set the current screen to being the start menu
	if (mCurrentScreen)
		delete mCurrentScreen;

	mCurrentScreen     = (BaseMenu*)(new StartMenu);

	mCurrentScreenType = SCREENS::MAIN_MENU;
	mPriorScreenType   = SCREENS::MAIN_MENU;

	mGameInstance      = nullptr;
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::Update(int elapsedTime)
{
	float deltaTime = ((float)elapsedTime / 1000.0f);

	// Update the current screen/menu and check if we are quitting the program
	SCREENS screenToSwapTo = mCurrentScreen->Update(deltaTime);
	bool swappingToPriorScreen = false;

	if (screenToSwapTo == SCREENS::PRIOR)
	{
		screenToSwapTo = mPriorScreenType;
		swappingToPriorScreen = true;
	}

	if (HandleScreenUpdate(screenToSwapTo))
	{
		// Close the program
		S2D::Audio::Destroy();
		S2D::Graphics::Destroy();
	}

	if (swappingToPriorScreen)
	{
		SCREENS temp       = mCurrentScreenType;
		mCurrentScreenType = mPriorScreenType;
		mPriorScreenType   = temp;
	}
}

// -------------------------------------------------------------------------------------------------------------- //

bool Pacman::HandleScreenUpdate(SCREENS newScreen)
{
	// Switch through the screens we want to transition to 
	switch (newScreen)
	{
	case SCREENS::HIGH_SCORES:
		// If we are not coming from the game then we can delete the current screen safely
		if (mCurrentScreenType != SCREENS::IN_GAME)
		{
			delete mCurrentScreen;
			mCurrentScreen = nullptr;
		}

		mPriorScreenType   = mCurrentScreenType;
		mCurrentScreenType = SCREENS::HIGH_SCORES;
		mCurrentScreen     = (BaseMenu*)(new HighScoresMenu);
	break;

	// Going into a new game
	case SCREENS::IN_GAME:
		// If a game currently exists then delete it
		if (mGameInstance)
		{
			delete mGameInstance;
			mGameInstance = nullptr;
		}
		else
		{
			delete mCurrentScreen;
		}

		mCurrentScreen     = nullptr;

		// Create a new game instance
		mGameInstance      = new MainGameScreen;

		mPriorScreenType   = mCurrentScreenType;
		mCurrentScreenType = SCREENS::IN_GAME;

		// Set the current screen to be our game instance
		mCurrentScreen     = (BaseMenu*)mGameInstance;
	break;

	case SCREENS::MAIN_MENU:
		// If we are going to the main menu then we need to clear all memory of a game instance
		if (mGameInstance)
		{
			delete mGameInstance;
			mGameInstance  = nullptr;
		}
		else
		{
			delete mCurrentScreen;
		}

		mCurrentScreen = nullptr;

		mPriorScreenType   = mCurrentScreenType;
		mCurrentScreenType = SCREENS::MAIN_MENU;
		mCurrentScreen     = (BaseMenu*)(new StartMenu);
	break;

	case SCREENS::PAUSE_MENU:
		if (mCurrentScreenType != SCREENS::IN_GAME)
		{
			delete mCurrentScreen;
			mCurrentScreen = nullptr;
		}

		mPriorScreenType   = mCurrentScreenType;
		mCurrentScreenType = SCREENS::PAUSE_MENU;
		mCurrentScreen     = (BaseMenu*)(new PauseMenu);
	break;

	case SCREENS::PRIOR:
	break;

	case SCREENS::SAME:
	break;

	case SCREENS::QUIT:
		return true;
	break;

	default:
	break;
	}

	return false;
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::Draw(int elapsedTime)
{
	// Starts Drawing
	SpriteBatch::BeginDraw();

		// Render the current screen/menu
		mCurrentScreen->Render(mFrameCount);

		// Increment the frame count
		mFrameCount++;

		if (mFrameCount >= FRAME_RATE)
			mFrameCount = 0;

	SpriteBatch::EndDraw(); // Ends Drawing
}

// -------------------------------------------------------------------------------------------------------------- //