#include "StartMenu.h"

#include "Constants.h"
#include "GameManager.h"

#include <iostream>

// -------------------------------------------------------- //

StartMenu::StartMenu() : mMaxSelectorSpriteFrames(2), mAmountOfRenderedFramesPerAnimationFrame(10), mAmountOfSpitesOnSelectorHeight(5), mAmountOfSpitesOnSelectorWidth(2)
{
	// Setup the text renderer
	mTextRenderer        = new TextRenderer("Textures/UI/Font.png", 15, 21);

	// Load in the selector sprite sheet
	mSelectorSpriteSheet = new S2D::Texture2D();
	mSelectorSpriteSheet->Load("Textures/SelectorSprite.png", false);
	if (!mSelectorSpriteSheet)
	{
		std::cout << "Failed to load the selector sprite for the main menu." << std::endl;
		return;
	}

	mSingleSpriteWidth              = float(mSelectorSpriteSheet->GetWidth() / mAmountOfSpitesOnSelectorWidth);
	mSingleSpriteHeight             = float(mSelectorSpriteSheet->GetHeight() / mAmountOfSpitesOnSelectorHeight);

	mSelectorSourceRenderRect       = new S2D::Rect(0, 0, (unsigned int)mSingleSpriteWidth, (unsigned int)mSingleSpriteHeight - 1);
	mSelectorSpriteCurrentFrame     = 0;

	mStartGamePosition              = S2D::Vector2(350, 250);
	mHighScoresPosition             = S2D::Vector2(350, 300);
	mChangeCharacterPosition        = S2D::Vector2(350, 350);
	mExitGamePosition               = S2D::Vector2(350, 400);

	mSelectorPosition               = S2D::Vector2(50, mStartGamePosition.Y - (mSingleSpriteHeight / 2));

	mCurrentlySelectedOption        = SELECTION_OPTIONS::START_GAME;
	mButtonCurrentlyPressed         = false;
}

// -------------------------------------------------------- //

StartMenu::~StartMenu()
{
	delete mTextRenderer;
	mTextRenderer = nullptr;

	delete mSelectorSpriteSheet;
	mSelectorSpriteSheet = nullptr;

	delete mSelectorSourceRenderRect;
	mSelectorSourceRenderRect = nullptr;

}

// -------------------------------------------------------- //

void StartMenu::Render(const unsigned int frameCount)
{
	if (mSelectorSpriteSheet)
	{
		// Handle which frame of animation the sprite is on
		if (frameCount % mAmountOfRenderedFramesPerAnimationFrame == 0)
		{
			mSelectorSpriteCurrentFrame++;
		}

		if (mSelectorSpriteCurrentFrame >= mMaxSelectorSpriteFrames)
			mSelectorSpriteCurrentFrame = 0;

		mSelectorSourceRenderRect->X = mSelectorSpriteCurrentFrame * mSingleSpriteWidth;

		// Now handle which sprite representation should be shown
		switch (GameManager::Instance()->GetPlayerCharacterType())
		{
		case PLAYER_CHARACTER_TYPE::RED_GHOST:
			mSelectorSourceRenderRect->Y = 0.0f;
		break;

		case PLAYER_CHARACTER_TYPE::BLUE_GHOST:
			mSelectorSourceRenderRect->Y = 2 * mSingleSpriteHeight;
		break;

		case PLAYER_CHARACTER_TYPE::PINK_GHOST:
			mSelectorSourceRenderRect->Y = mSingleSpriteHeight;
		break;

		case PLAYER_CHARACTER_TYPE::ORANGE_GHOST:
			mSelectorSourceRenderRect->Y = 3 * mSingleSpriteHeight;
		break;

		case PLAYER_CHARACTER_TYPE::PACMAN:
			mSelectorSourceRenderRect->Y = 4 * mSingleSpriteHeight;
		break;
		}

		S2D::SpriteBatch::Draw(mSelectorSpriteSheet, &mSelectorPosition, mSelectorSourceRenderRect);
	}

	mTextRenderer->Render("START GAME",       20, mStartGamePosition,       2);
	mTextRenderer->Render("HIGHSCORES",       20, mHighScoresPosition,      2);
	mTextRenderer->Render("CHANGE CHARACTER", 30, mChangeCharacterPosition, 2);
	mTextRenderer->Render("EXIT GAME",        20, mExitGamePosition,        2);
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

			mSelectorPosition.Y = mHighScoresPosition.Y - (mSingleSpriteHeight / 2);
		break;

		case SELECTION_OPTIONS::HIGHSCORES:
			mCurrentlySelectedOption = SELECTION_OPTIONS::CHANGE_PLAYER;

			mSelectorPosition.Y = mChangeCharacterPosition.Y - (mSingleSpriteHeight / 2);
		break;

		case SELECTION_OPTIONS::CHANGE_PLAYER:
			mCurrentlySelectedOption = SELECTION_OPTIONS::QUIT;

			mSelectorPosition.Y = mExitGamePosition.Y - (mSingleSpriteHeight / 2);
		break;

		case SELECTION_OPTIONS::QUIT:
			mCurrentlySelectedOption = SELECTION_OPTIONS::START_GAME;

			mSelectorPosition.Y = mStartGamePosition.Y - (mSingleSpriteHeight / 2);
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

			mSelectorPosition.Y = mExitGamePosition.Y - (mSingleSpriteHeight / 2);
		break;

		case SELECTION_OPTIONS::HIGHSCORES:
			mCurrentlySelectedOption = SELECTION_OPTIONS::START_GAME;

			mSelectorPosition.Y = mStartGamePosition.Y - (mSingleSpriteHeight / 2);
		break;

		case SELECTION_OPTIONS::CHANGE_PLAYER:
			mCurrentlySelectedOption = SELECTION_OPTIONS::HIGHSCORES;

			mSelectorPosition.Y = mHighScoresPosition.Y - (mSingleSpriteHeight / 2);
		break;

		case SELECTION_OPTIONS::QUIT:
			mCurrentlySelectedOption = SELECTION_OPTIONS::CHANGE_PLAYER;

			mSelectorPosition.Y = mChangeCharacterPosition.Y - (mSingleSpriteHeight / 2);
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