#include "StartMenu.h"

#include "Constants.h"

#include <iostream>

// -------------------------------------------------------- //

StartMenu::StartMenu()
{
	mBackgroundSprite = new S2D::Texture2D();
	mBackgroundSprite->Load("Textures/Backgrounds/StartBackground.png", false);
	if (!mBackgroundSprite)
	{
		std::cout << "Failed to load the background sprite for the main menu." << std::endl;
		return;
	}

	mBackgroundRenderRect = new S2D::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	mSelectorSprite = new S2D::Texture2D();
	mSelectorSprite->Load("Textures/SelectorSprite.png", false);
	if (!mSelectorSprite)
	{
		std::cout << "Failed to load the selector sprite for the main menu." << std::endl;
		return;
	}

	mSelectorRenderRect       = new S2D::Rect(0, 0, mSelectorSprite->GetWidth(), mSelectorSprite->GetHeight());

	mCurrentlySelectedOption  = SELECTION_OPTIONS::START_GAME;
	mButtonCurrentlyPressed   = false;

	// Initialse the positions for the text to be rendered in the start menu
	mStartOptionPosition      = new S2D::Vector2(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT + 40);
	mHighScoresOptionPosition = new S2D::Vector2(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT + 80);
	mQuitOptionPosition       = new S2D::Vector2(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT + 120);
	mStartMenuLabelPosition   = new S2D::Vector2(HALF_SCREEN_WIDTH - 60, QUATER_SCREEN_HEIGHT);

	mSelectorPosition         = new S2D::Vector2(mStartOptionPosition->X - 50, mStartOptionPosition->Y - mSelectorSprite->GetWidth());
}

// -------------------------------------------------------- //

StartMenu::~StartMenu()
{
	delete mBackgroundSprite;
		mBackgroundSprite = nullptr;

	delete mSelectorSprite;
		mSelectorSprite = nullptr;

	delete mBackgroundRenderRect;
		mBackgroundRenderRect = nullptr;

	delete mSelectorRenderRect;
		mSelectorRenderRect = nullptr;

	delete mStartOptionPosition;
		mStartOptionPosition = nullptr;

	delete mHighScoresOptionPosition;
		mHighScoresOptionPosition = nullptr;

	delete mQuitOptionPosition;
		mQuitOptionPosition = nullptr;

	delete mStartMenuLabelPosition;
		mStartMenuLabelPosition = nullptr;
}

// -------------------------------------------------------- //

void StartMenu::Render()
{
	if(mBackgroundSprite && mBackgroundRenderRect)
		S2D::SpriteBatch::Draw(mBackgroundSprite, mBackgroundRenderRect);

	if(mSelectorSprite && mSelectorPosition && mSelectorRenderRect)
		S2D::SpriteBatch::Draw(mSelectorSprite, mSelectorPosition, mSelectorRenderRect);

	if(mStartOptionPosition)
		S2D::SpriteBatch::DrawString("START GAME", mStartOptionPosition, S2D::Color::White);

	if(mHighScoresOptionPosition)
		S2D::SpriteBatch::DrawString("HIGH SCORES", mHighScoresOptionPosition, S2D::Color::White);

	if(mQuitOptionPosition)
		S2D::SpriteBatch::DrawString("QUIT", mQuitOptionPosition, S2D::Color::White);

	if(mStartMenuLabelPosition)
		S2D::SpriteBatch::DrawString("START MENU:", mStartMenuLabelPosition, S2D::Color::White);
}

// -------------------------------------------------------- //

SELECTION_OPTIONS StartMenu::Update(const float deltaTime)
{
	// Check for any relevent button inputs
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();
	if (keyboardState->IsKeyDown(S2D::Input::Keys::DOWN) && !mButtonCurrentlyPressed)
	{
		switch (mCurrentlySelectedOption)
		{
		case SELECTION_OPTIONS::START_GAME:
			mCurrentlySelectedOption = SELECTION_OPTIONS::HIGHSCORES;

			mSelectorPosition->Y     = mHighScoresOptionPosition->Y - mSelectorSprite->GetWidth();
		break;

		case SELECTION_OPTIONS::HIGHSCORES:
			mCurrentlySelectedOption = SELECTION_OPTIONS::QUIT;

			mSelectorPosition->Y     = mQuitOptionPosition->Y - mSelectorSprite->GetWidth();
		break;

		case SELECTION_OPTIONS::QUIT:
			mCurrentlySelectedOption = SELECTION_OPTIONS::START_GAME;

			mSelectorPosition->Y     = mStartOptionPosition->Y - mSelectorSprite->GetWidth();
		break;

		default:
			mCurrentlySelectedOption = SELECTION_OPTIONS::QUIT;

			mSelectorPosition->Y = mQuitOptionPosition->Y - mSelectorSprite->GetWidth();
		break;
		}

		mButtonCurrentlyPressed = true;
		return SELECTION_OPTIONS::NONE;
		//
	}
	else if (keyboardState->IsKeyDown(S2D::Input::Keys::UP) && !mButtonCurrentlyPressed)
	{
		switch (mCurrentlySelectedOption)
		{
		case SELECTION_OPTIONS::START_GAME:
			mCurrentlySelectedOption = SELECTION_OPTIONS::QUIT;

			mSelectorPosition->Y     = mQuitOptionPosition->Y - mSelectorSprite->GetWidth();
		break;

		case SELECTION_OPTIONS::HIGHSCORES:
			mCurrentlySelectedOption = SELECTION_OPTIONS::START_GAME;

			mSelectorPosition->Y     = mStartOptionPosition->Y - mSelectorSprite->GetWidth();
		break;

		case SELECTION_OPTIONS::QUIT:
			mCurrentlySelectedOption = SELECTION_OPTIONS::HIGHSCORES;

			mSelectorPosition->Y     = mHighScoresOptionPosition->Y - mSelectorSprite->GetWidth();
		break;

		default:
			mCurrentlySelectedOption = SELECTION_OPTIONS::QUIT;

			mSelectorPosition->Y     = mQuitOptionPosition->Y - mSelectorSprite->GetWidth();
		break;
		}

		mButtonCurrentlyPressed = true;
		return SELECTION_OPTIONS::NONE;
	}

	if (keyboardState->IsKeyDown(S2D::Input::Keys::RETURN) && !mButtonCurrentlyPressed)
	{
		// Return the currently selected option
		mButtonCurrentlyPressed = true;

		return mCurrentlySelectedOption;
	}

	// Now check if all buttons we care about are released
	if (keyboardState->IsKeyUp(S2D::Input::Keys::DOWN) && keyboardState->IsKeyUp(S2D::Input::Keys::UP) && keyboardState->IsKeyUp(S2D::Input::Keys::RETURN))
	{
		mButtonCurrentlyPressed = false;
	}

	return SELECTION_OPTIONS::NONE;
}

// -------------------------------------------------------- //