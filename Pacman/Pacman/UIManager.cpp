#include "UIManager.h"

#include "GameManager.h"

#include <iostream>

// ---------------------------------------------------------------- //

UIManager* UIManager::mInstance = nullptr;

// ---------------------------------------------------------------- //

UIManager::UIManager()
{
	// First load in the sprite sheets we need
	mExtraLivesSpriteSheet       = new S2D::Texture2D();
	mPointsSpriteSheet           = new S2D::Texture2D();
	mCurrentScoreFontSpriteSheet = new S2D::Texture2D();

	mExtraLivesSpriteSheet->Load("Textures/UI/LifeIcon.png", false);
	mPointsSpriteSheet->Load("Textures/UI/PointsSpriteSheet.png", false);
	mCurrentScoreFontSpriteSheet->Load("Textures/UI/ColouredFonts.png", false);

	// Check that the sprite sheets have been loaded in correctly
	if (!mExtraLivesSpriteSheet)
		std::cout << "Failed to load in the sprite sheet for the Lives UI icon." << std::endl;

	if (!mPointsSpriteSheet)
		std::cout << "Failed to load in the sprite sheet for the UI points." << std::endl;

	if (!mCurrentScoreFontSpriteSheet)
		std::cout << "Failed to load in the font sprite sheet for the UI manager." << std::endl;

	// Set the display positions
	mCollectedPickUpsPosition    = S2D::Vector2();
	mExtraLivesStartTopRightPos  = S2D::Vector2(125, 600);
	mHighScoreDisplayPosition    = S2D::Vector2();
	mCurrentScoreDisplayPosition = S2D::Vector2();

	// Setup the extra life render rect
	mExtraLifeRenderRect         = new S2D::Rect(0, 0, mExtraLivesSpriteSheet->GetWidth(), mExtraLivesSpriteSheet->GetHeight());
}

// ---------------------------------------------------------------- //

UIManager::~UIManager()
{
	delete mExtraLivesSpriteSheet;
	mExtraLivesSpriteSheet = nullptr;

	delete mPointsSpriteSheet;
	mPointsSpriteSheet = nullptr;

	delete mCurrentScoreFontSpriteSheet;
	mCurrentScoreFontSpriteSheet = nullptr;

	delete mExtraLifeRenderRect;
	mExtraLifeRenderRect = nullptr;

	delete mInstance;
	mInstance = nullptr;
}

// ---------------------------------------------------------------- //

void UIManager::Render()
{
	// First render the current score to the screen


	// Now render the current highscore to the screen



	// Now render the extra lives icons
	for (unsigned int lifeID = 0; lifeID < GameManager::Instance()->GetExtraLivesCount(); lifeID++)
	{
		S2D::Vector2* position = new S2D::Vector2(mExtraLivesStartTopRightPos - S2D::Vector2(35.0f * lifeID, 0.0f));

		S2D::SpriteBatch::Draw(mExtraLivesSpriteSheet, 
							   position,
			                   mExtraLifeRenderRect);

		delete position;
	}

	// Now render the collected fruit icons
	for (unsigned int i = 0; i < mCollectedPickups.size(); i++)
	{
		mCollectedPickups[i].Render(mCollectedPickUpsPosition - S2D::Vector2(50, 0.0f));
	}
}

// ---------------------------------------------------------------- //

UIManager* UIManager::GetInstance()
{
	if (!mInstance)
		mInstance = new UIManager;

	return mInstance;
}

// ---------------------------------------------------------------- //