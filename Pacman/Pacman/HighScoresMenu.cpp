#include "HighScoresMenu.h"

#include "Constants.h"

#include <iostream>

// ---------------------------------------------------------------- //

HighScoresMenu::HighScoresMenu()
{
	mBackgroundSprite = new S2D::Texture2D();
	mBackgroundSprite->Load("Textures/Backgrounds/HighScoresBackground.png", false);
	if (!mBackgroundSprite)
	{
		std::cout << "Failed to load the background sprite for the high scores screen." << std::endl;
		return;
	}

	mRenderRect = new S2D::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

// ---------------------------------------------------------------- //

HighScoresMenu::~HighScoresMenu()
{

}

// ---------------------------------------------------------------- //

void HighScoresMenu::Render()
{
	if(mBackgroundSprite && mRenderRect)
		S2D::SpriteBatch::Draw(mBackgroundSprite, mRenderRect);
}

// ---------------------------------------------------------------- //

bool HighScoresMenu::Update()
{
	// Allow the player to return back to the previous screen
	if (S2D::Input::Keyboard::GetState()->IsKeyDown(S2D::Input::Keys::ESCAPE))
	{
		return false;
	}

	return true;
}

// ---------------------------------------------------------------- //