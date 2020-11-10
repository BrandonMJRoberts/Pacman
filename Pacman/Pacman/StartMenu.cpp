#include "StartMenu.h"

#include "Constants.h"
#include "GameManager.h"

#include "TextRenderer.h"

#include <string>
#include <vector>
#include <iostream>


// -------------------------------------------------------- //

StartMenu::StartMenu() : BaseMenu(), mMaxSelectorSpriteFrames(2), mAmountOfRenderedFramesPerAnimationFrame(10), mAmountOfSpitesOnSelectorHeight(5), mAmountOfSpitesOnSelectorWidth(2)
{
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
	mSetNamePosition                = S2D::Vector2(350, 400);
	mExitGamePosition               = S2D::Vector2(350, 450);

	mSelectorPosition               = S2D::Vector2(50, mStartGamePosition.Y - (mSingleSpriteHeight / 2));

	mCurrentlySelectedOption        = SELECTION_OPTIONS::START_GAME;
	mButtonCurrentlyPressed         = false;
	mLettersPressedCurrently        = false;

	// Setup the text renderer
	mTextRenderer                   = new TextRenderer("Textures/UI/Font.png", 15, 21);

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
	mTextRenderer->Render("NAME - " + GameManager::Instance()->GetPlayerName(),      40, mSetNamePosition,         GameManager::Instance()->GetPlayerNameColourIndex());
	mTextRenderer->Render("EXIT GAME",        20, mExitGamePosition,        2);
}

// -------------------------------------------------------- //

SCREENS StartMenu::Update(const float deltaTime)
{
	// Check for any relevent button inputs
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	// Traversing the menu's
	HandleDownPress(*keyboardState);
	HandleUpPress(*keyboardState);

	// Now check if all buttons we care about are released
	if (keyboardState->IsKeyUp(S2D::Input::Keys::DOWN) && 
		keyboardState->IsKeyUp(S2D::Input::Keys::UP)   && 
		keyboardState->IsKeyUp(S2D::Input::Keys::RETURN))
	{
		mButtonCurrentlyPressed = false;
	}

	if (mCurrentlySelectedOption != SELECTION_OPTIONS::CHANGE_NAME)
	{
		// Get if the return button has been pressed
		SELECTION_OPTIONS buttonPress = HandleReturnPress(*keyboardState);
		if (buttonPress != SELECTION_OPTIONS::NONE)
		{
			switch (buttonPress)
			{
			case SELECTION_OPTIONS::START_GAME:
				return SCREENS::IN_GAME;
				break;

			case SELECTION_OPTIONS::HIGHSCORES:
				return SCREENS::HIGH_SCORES;
				break;

			case SELECTION_OPTIONS::QUIT:
				return SCREENS::QUIT;
				break;

			case SELECTION_OPTIONS::CHANGE_PLAYER:
				GameManager::Instance()->IncrementPlayerCharacter();
				break;

			default:
				return SCREENS::SAME;
				break;
			}
		}
		else
			return SCREENS::SAME;
	}
	else
	{
		// We are on the change name option so now we need to take input of letters from the keyboard in order to enter a name
		TypingInName();
	}

	return SCREENS::SAME;
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
			mCurrentlySelectedOption = SELECTION_OPTIONS::CHANGE_NAME;

			mSelectorPosition.Y = mSetNamePosition.Y - (mSingleSpriteHeight / 2);
		break;

		case SELECTION_OPTIONS::CHANGE_NAME:
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

		case SELECTION_OPTIONS::CHANGE_NAME:
			mCurrentlySelectedOption = SELECTION_OPTIONS::CHANGE_PLAYER;

			mSelectorPosition.Y = mChangeCharacterPosition.Y - (mSingleSpriteHeight / 2);
		break;

		case SELECTION_OPTIONS::QUIT:
			mCurrentlySelectedOption = SELECTION_OPTIONS::CHANGE_NAME;

			mSelectorPosition.Y = mSetNamePosition.Y - (mSingleSpriteHeight / 2);
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

void StartMenu::TypingInName()
{
	// First get what is currently displayed
	string currentName = GameManager::Instance()->GetPlayerName();

	// Only take in certain characters in certain situations
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	if (keyboardState->GetPressedKeys().size() == 0)
	{
		mLettersPressedCurrently = false;
	}

	if (!mLettersPressedCurrently)
	{
		// First check for the backspace
		if (keyboardState->IsKeyDown(S2D::Input::Keys::BACKSPACE) && currentName.size() > 0)
		{
			currentName.pop_back();
			GameManager::Instance()->SetPlayerName(currentName);
			mLettersPressedCurrently = true;
			return;
		}

		if (keyboardState->IsKeyDown(S2D::Input::Keys::DOWN) || keyboardState->IsKeyDown(S2D::Input::Keys::UP))
		{
			return;
		}
		else if (keyboardState->IsKeyDown(S2D::Input::Keys::RETURN))
		{
			mLettersPressedCurrently = true;
			GameManager::Instance()->IncrementPlayerColourIndex();
		}

		// Only add the first character in the vector to the name
		if (currentName.size() < 7 && keyboardState->GetPressedKeys().size() > 0)
		{
			mLettersPressedCurrently = true;

			switch (keyboardState->GetPressedKeys()[0])
			{
			case S2D::Input::Keys::A: currentName.push_back('A'); break;
			case S2D::Input::Keys::B: currentName.push_back('B'); break;
			case S2D::Input::Keys::C: currentName.push_back('C'); break;
			case S2D::Input::Keys::D: currentName.push_back('D'); break;
			case S2D::Input::Keys::E: currentName.push_back('E'); break;
			case S2D::Input::Keys::F: currentName.push_back('F'); break;
			case S2D::Input::Keys::G: currentName.push_back('G'); break;
			case S2D::Input::Keys::H: currentName.push_back('H'); break;
			case S2D::Input::Keys::I: currentName.push_back('I'); break;
			case S2D::Input::Keys::J: currentName.push_back('J'); break;
			case S2D::Input::Keys::K: currentName.push_back('K'); break;
			case S2D::Input::Keys::L: currentName.push_back('L'); break;
			case S2D::Input::Keys::M: currentName.push_back('M'); break;
			case S2D::Input::Keys::N: currentName.push_back('N'); break;
			case S2D::Input::Keys::O: currentName.push_back('O'); break;
			case S2D::Input::Keys::P: currentName.push_back('P'); break;
			case S2D::Input::Keys::Q: currentName.push_back('Q'); break;
			case S2D::Input::Keys::R: currentName.push_back('R'); break;
			case S2D::Input::Keys::S: currentName.push_back('S'); break;
			case S2D::Input::Keys::T: currentName.push_back('T'); break;
			case S2D::Input::Keys::U: currentName.push_back('U'); break;
			case S2D::Input::Keys::V: currentName.push_back('V'); break;
			case S2D::Input::Keys::W: currentName.push_back('W'); break;
			case S2D::Input::Keys::X: currentName.push_back('X'); break;
			case S2D::Input::Keys::Y: currentName.push_back('Y'); break;
			case S2D::Input::Keys::Z: currentName.push_back('Z'); break;
			default:	break;
			}

			GameManager::Instance()->SetPlayerName(currentName);
		}
	}
}

// -------------------------------------------------------- //