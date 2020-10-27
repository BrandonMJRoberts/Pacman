#include "StartMenu.h"

#include "Constants.h"
#include "GameManager.h"

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

	mAmountOfSpitesOnSelectorWidth  = 2;
	mAmountOfSpitesOnSelectorHeight = 5;

	mSelectorRenderRect             = new S2D::Rect(0, 
		                                            0, 
		                                            mSelectorSprite->GetWidth() / mAmountOfSpitesOnSelectorWidth, 
		                                            mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);

	mCurrentlySelectedOption        = SELECTION_OPTIONS::START_GAME;
	mButtonCurrentlyPressed         = false;

	// Initialse the positions for the text to be rendered in the start menu
	mStartOptionPosition            = new S2D::Vector2     (HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT        + 40);
	mHighScoresOptionPosition       = new S2D::Vector2     (HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT        + 80);
	mChangeCharacterOptionPosition  = new S2D::Vector2     (HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT        + 120);
	mQuitOptionPosition			    = new S2D::Vector2     (HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT        + 160);
	mStartMenuLabelPosition		    = new S2D::Vector2     (HALF_SCREEN_WIDTH - 60, QUATER_SCREEN_HEIGHT);

	mSelectorPosition               = new S2D::Vector2(mStartOptionPosition->X - (2 * (mSelectorSprite->GetWidth() / mAmountOfSpitesOnSelectorWidth)),
		                                               mStartOptionPosition->Y -      (mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight));
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

	delete mChangeCharacterOptionPosition;
		delete mChangeCharacterOptionPosition;

	delete mStartMenuLabelPosition;
		mStartMenuLabelPosition = nullptr;
}

// -------------------------------------------------------- //

void StartMenu::Render(const unsigned int frameCount)
{
	if(mBackgroundSprite && mBackgroundRenderRect)
		S2D::SpriteBatch::Draw(mBackgroundSprite, mBackgroundRenderRect);

	if (mSelectorSprite && mSelectorPosition && mSelectorRenderRect)
	{
		// Handle which frame of animation the sprite is on
		if (frameCount % 20 < 10)
			mSelectorRenderRect->X = 0.0f;
		else 
			mSelectorRenderRect->X = float(mSelectorSprite->GetWidth() / mAmountOfSpitesOnSelectorWidth);

		// Now handle which sprite representation should be shown
		switch (GameManager::Instance()->GetPlayerCurrentCharacter())
		{
		case PLAYER_CHARACTER_TYPE::BLUE_GHOST:
			mSelectorRenderRect->Y = 2 * float(mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		case PLAYER_CHARACTER_TYPE::RED_GHOST:
			mSelectorRenderRect->Y = 0.0f;
		break;

		case PLAYER_CHARACTER_TYPE::PINK_GHOST:
			mSelectorRenderRect->Y = float(mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		case PLAYER_CHARACTER_TYPE::ORANGE_GHOST:
			mSelectorRenderRect->Y = 3 * float(mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		case PLAYER_CHARACTER_TYPE::PACMAN:
			mSelectorRenderRect->Y = 4 * float(mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;
		}

		S2D::SpriteBatch::Draw(mSelectorSprite, mSelectorPosition, mSelectorRenderRect);
	}

	if(mStartOptionPosition)
		S2D::SpriteBatch::DrawString("START GAME", mStartOptionPosition, S2D::Color::White);

	if(mHighScoresOptionPosition)
		S2D::SpriteBatch::DrawString("HIGH SCORES", mHighScoresOptionPosition, S2D::Color::White);

	if(mChangeCharacterOptionPosition)
		S2D::SpriteBatch::DrawString("CHANGE CHARACTER", mChangeCharacterOptionPosition, S2D::Color::White);

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

	HandleDownPress(*keyboardState);
	HandleUpPress(*keyboardState);

	SELECTION_OPTIONS buttonPress = HandleReturnPress(*keyboardState);
	if (buttonPress != SELECTION_OPTIONS::NONE)
		return buttonPress;

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

void StartMenu::HandleDownPress(S2D::Input::KeyboardState& keyboardState)
{
	if (keyboardState.IsKeyDown(S2D::Input::Keys::DOWN) && !mButtonCurrentlyPressed)
	{
		switch (mCurrentlySelectedOption)
		{
		case SELECTION_OPTIONS::START_GAME:
			mCurrentlySelectedOption = SELECTION_OPTIONS::HIGHSCORES;

			mSelectorPosition->Y = mHighScoresOptionPosition->Y - (mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		case SELECTION_OPTIONS::HIGHSCORES:
			mCurrentlySelectedOption = SELECTION_OPTIONS::CHANGE_PLAYER;

			mSelectorPosition->Y = mChangeCharacterOptionPosition->Y - (mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		case SELECTION_OPTIONS::CHANGE_PLAYER:
			mCurrentlySelectedOption = SELECTION_OPTIONS::QUIT;

			mSelectorPosition->Y = mQuitOptionPosition->Y - (mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		case SELECTION_OPTIONS::QUIT:
			mCurrentlySelectedOption = SELECTION_OPTIONS::START_GAME;

			mSelectorPosition->Y = mStartOptionPosition->Y - (mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		default:
		break;
		}

		mButtonCurrentlyPressed = true;
	}
}

// -------------------------------------------------------- //

void StartMenu::HandleUpPress(S2D::Input::KeyboardState& keyboardState)
{
	if (keyboardState.IsKeyDown(S2D::Input::Keys::UP) && !mButtonCurrentlyPressed)
	{
		switch (mCurrentlySelectedOption)
		{
		case SELECTION_OPTIONS::START_GAME:
			mCurrentlySelectedOption = SELECTION_OPTIONS::QUIT;

			mSelectorPosition->Y = mQuitOptionPosition->Y - (mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		case SELECTION_OPTIONS::HIGHSCORES:
			mCurrentlySelectedOption = SELECTION_OPTIONS::START_GAME;

			mSelectorPosition->Y = mStartOptionPosition->Y - (mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		case SELECTION_OPTIONS::CHANGE_PLAYER:
			mCurrentlySelectedOption = SELECTION_OPTIONS::HIGHSCORES;

			mSelectorPosition->Y = mHighScoresOptionPosition->Y - (mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		case SELECTION_OPTIONS::QUIT:
			mCurrentlySelectedOption = SELECTION_OPTIONS::CHANGE_PLAYER;

			mSelectorPosition->Y = mChangeCharacterOptionPosition->Y - (mSelectorSprite->GetHeight() / mAmountOfSpitesOnSelectorHeight);
		break;

		default:
		break;
		}

		mButtonCurrentlyPressed = true;
	}
}

// -------------------------------------------------------- //

SELECTION_OPTIONS StartMenu::HandleReturnPress(S2D::Input::KeyboardState& keyboardState)
{
	if (keyboardState.IsKeyDown(S2D::Input::Keys::RETURN) && !mButtonCurrentlyPressed)
	{
		// Return the currently selected option
		mButtonCurrentlyPressed = true;

		return mCurrentlySelectedOption;
	}

	return SELECTION_OPTIONS::NONE;
}

// -------------------------------------------------------- //