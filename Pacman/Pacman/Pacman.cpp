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
	srand(time(NULL));

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
}

// -------------------------------------------------------------------------------------------------------------- //

void Pacman::Update(int elapsedTime)
{
	float deltaTime = ((float)elapsedTime / 1000.0f);

	// Update the current screen/menu and check if we are quitting the program
	if (HandleScreenUpdate(mCurrentScreen->Update(deltaTime)))
	{
		// Close the program
		S2D::Audio::Destroy();
		S2D::Graphics::Destroy();
	}
}

// -------------------------------------------------------------------------------------------------------------- //

bool Pacman::HandleScreenUpdate(SCREENS newScreen)
{
	SCREENS temp;

	switch (newScreen)
	{
	case SCREENS::HIGH_SCORES:
		delete mCurrentScreen;

		mPriorScreenType   = mCurrentScreenType;
		mCurrentScreenType = SCREENS::HIGH_SCORES;
		mCurrentScreen     = (BaseMenu*)(new HighScoresMenu);
	break;

	case SCREENS::IN_GAME:
		delete mCurrentScreen;

		mPriorScreenType   = mCurrentScreenType;
		mCurrentScreenType = SCREENS::IN_GAME;
		mCurrentScreen     = (BaseMenu*)(new MainGameScreen);
	break;

	case SCREENS::MAIN_MENU:
		delete mCurrentScreen;

		mPriorScreenType   = mCurrentScreenType;
		mCurrentScreenType = SCREENS::MAIN_MENU;
		mCurrentScreen     = (BaseMenu*)(new StartMenu);
	break;

	case SCREENS::PAUSE_MENU:
		delete mCurrentScreen;

		mPriorScreenType   = mCurrentScreenType;
		mCurrentScreenType = SCREENS::PAUSE_MENU;
		mCurrentScreen     = (BaseMenu*)(new PauseMenu);
	break;

	case SCREENS::PRIOR:
		temp               = mCurrentScreenType;

		mCurrentScreenType = mPriorScreenType;
		mPriorScreenType   = temp;

		switch (mCurrentScreenType)
		{
		case SCREENS::HIGH_SCORES:
			delete mCurrentScreen;

			mCurrentScreen = (BaseMenu*)(new HighScoresMenu);
		break;

		case SCREENS::IN_GAME:
			delete mCurrentScreen;

			mCurrentScreen = (BaseMenu*)(new MainGameScreen);
		break;

		case SCREENS::MAIN_MENU:
			delete mCurrentScreen;

			mCurrentScreen = (BaseMenu*)(new StartMenu);
		break;

		case SCREENS::PAUSE_MENU:
			delete mCurrentScreen;

			mCurrentScreen = (BaseMenu*)(new PauseMenu);
		break;
		}
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