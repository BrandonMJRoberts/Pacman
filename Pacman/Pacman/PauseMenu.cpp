#include "PauseMenu.h"

#include "GameManager.h"
#include "Constants.h"

#include "TextRenderer.h"

#include <iostream>

// ---------------------------------------------------------------------- //

PauseMenu::PauseMenu() : BaseMenu()
{
	mBackgroundSprite = new S2D::Texture2D();
	mBackgroundSprite->Load("Textures/Backgrounds/PausedBackground.png", false);
	if (!mBackgroundSprite)
	{
		std::cout << "Failed to load the background sprite for the pause menu." << std::endl;
	}

	mRenderRectangle = new S2D::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	mTextRenderer = new TextRenderer("Textures/UI/Font.png", 15, 21);
	if (!mTextRenderer)
	{
		std::cout << "Failed to create the text renderer for the pause menu" << std::endl;
		return;
	}

	mPauseMenuTextPosition                  = S2D::Vector2(HALF_SCREEN_WIDTH + (4 * SPRITE_RESOLUTION), 100);                                                          // PAUSE MENU
	mCurrentScoreTextPosition               = S2D::Vector2(QUATER_SCREEN_WIDTH / 4, mPauseMenuTextPosition.Y           + (2 * TWICE_SPRITE_RESOLUTION)); // CURRENT SCORE: SCORE
	mHighScoreTextPosition                  = S2D::Vector2(QUATER_SCREEN_WIDTH / 4, mCurrentScoreTextPosition.Y        + TWICE_SPRITE_RESOLUTION); // HIGH SCORE: SCORE
	mRemainingLivesCountTextPosition        = S2D::Vector2(QUATER_SCREEN_WIDTH / 4, mHighScoreTextPosition.Y           + TWICE_SPRITE_RESOLUTION); // EXTRA LIVES: count
	mPlayerNameTextPosition                 = S2D::Vector2(QUATER_SCREEN_WIDTH / 4, mRemainingLivesCountTextPosition.Y + TWICE_SPRITE_RESOLUTION); // NAME: name
	mGhostsEatenThisGameTextPosition        = S2D::Vector2(QUATER_SCREEN_WIDTH / 4, mPlayerNameTextPosition.Y          + TWICE_SPRITE_RESOLUTION); // GHOSTS EATEN: count
	mDotsCollectedThisGameCountTextPosition = S2D::Vector2(QUATER_SCREEN_WIDTH / 4, mGhostsEatenThisGameTextPosition.Y + TWICE_SPRITE_RESOLUTION); // DOTS COLLECTED: count
}

// ---------------------------------------------------------------------- //

PauseMenu::~PauseMenu()
{
	delete mBackgroundSprite;
	mBackgroundSprite = nullptr;

	delete mRenderRectangle;
	mRenderRectangle = nullptr;

	delete mTextRenderer;
	mTextRenderer = nullptr;
}

// ---------------------------------------------------------------------- //

void PauseMenu::Render(unsigned int frameCount)
{
	// Render the background sprite first
	if(mBackgroundSprite && mRenderRectangle)
		S2D::SpriteBatch::Draw(mBackgroundSprite, mRenderRectangle);

	// Render current stats to the screen
	if (mTextRenderer)
	{
		GameManager* GM = GameManager::Instance();

		mTextRenderer->RenderFromRight("PAUSE MENU",                                                   30, mPauseMenuTextPosition,                  0);
		mTextRenderer->RenderFromLeft("CURRENT SCORE   - " + std::to_string(GM->GetCurrentScore()),    30, mCurrentScoreTextPosition,               0);
		mTextRenderer->RenderFromLeft("HIGH SCORE      - " + std::to_string(GM->GetCurrentHighScore()),30, mHighScoreTextPosition,                  0);
		mTextRenderer->RenderFromLeft("REMAINING LIVES - " + std::to_string(GM->GetExtraLivesCount()), 30, mRemainingLivesCountTextPosition,        0);
		mTextRenderer->RenderFromLeft("NAME            - " + GM->GetPlayerName(),                      30, mPlayerNameTextPosition,                 0);
		mTextRenderer->RenderFromLeft("GHOSTS EATEN    - " + std::to_string(GM->GetAmountOfGhostsEatenthisGame()),                                           30, mGhostsEatenThisGameTextPosition,        0);
		mTextRenderer->RenderFromLeft("DOTS COLLECTED  - " + std::to_string(GM->GetCurrentAmountOfDotsEaten()),                                           30, mDotsCollectedThisGameCountTextPosition, 0);
	}
}

// ---------------------------------------------------------------------- //

SCREENS PauseMenu::Update(const float deltaTime)
{
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	// Do checks to see if the player has re-entered the game
	if (keyboardState->IsKeyDown(S2D::Input::Keys::P) && !GameManager::Instance()->GetIsAlreadySettingGamePaused())
	{
		GameManager::Instance()->SetGameIsPaused(false);
		GameManager::Instance()->SetIsPausedButtonPressed(true);

		return SCREENS::PRIOR;
	}
	else if (keyboardState->IsKeyUp(S2D::Input::Keys::P))
	{
		GameManager::Instance()->SetIsPausedButtonPressed(false);
	}

	return SCREENS::SAME;
}

// ---------------------------------------------------------------------- //