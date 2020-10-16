#include "PauseMenu.h"

#include "GameManager.h"
#include "Constants.h"

#include <iostream>

// ---------------------------------------------------------------------- //

PauseMenu::PauseMenu()
{
	mBackgroundSprite = new S2D::Texture2D();
	mBackgroundSprite->Load("Textures/Backgrounds/PausedBackground.png", false);
	if (!mBackgroundSprite)
	{
		std::cout << "Failed to load the background sprite for the pause menu." << std::endl;
	}

	mRenderRectangle = new S2D::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

// ---------------------------------------------------------------------- //

PauseMenu::~PauseMenu()
{
	delete mBackgroundSprite;
	mBackgroundSprite = nullptr;

	delete mRenderRectangle;
	mRenderRectangle = nullptr;
}

// ---------------------------------------------------------------------- //

void PauseMenu::Render()
{
	// Render the background sprite first
	if(mBackgroundSprite && mRenderRectangle)
		S2D::SpriteBatch::Draw(mBackgroundSprite, mRenderRectangle);
}

// ---------------------------------------------------------------------- //

void PauseMenu::Update(const float deltaTime)
{
	S2D::Input::KeyboardState* keyboardState = S2D::Input::Keyboard::GetState();

	// Do checks to see if the player has re-entered the game
	if (keyboardState->IsKeyDown(S2D::Input::Keys::P) && !GameManager::Instance()->GetIsAlreadySettingGamePaused())
	{
		GameManager::Instance()->SetGameIsPaused(false);
		GameManager::Instance()->SetIsPausedButtonPressed(true);
		return;
	}
	else if (keyboardState->IsKeyUp(S2D::Input::Keys::P))
	{
		GameManager::Instance()->SetIsPausedButtonPressed(false);
	}
}

// ---------------------------------------------------------------------- //