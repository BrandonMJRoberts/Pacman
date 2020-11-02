#include "UIManager.h"

#include "GameManager.h"

#include <iostream>

// ---------------------------------------------------------------- //

UIManager* UIManager::mInstance = nullptr;

// ---------------------------------------------------------------- //

UIManager::UIManager() : mAmountOfSpritesOnPointsSpriteSheetHeight(3), mAmountOfSpritesOnPointsSpriteSheetWidth(4)
{
	// First load in the sprite sheets we need
	mExtraLivesSpriteSheet       = new S2D::Texture2D();
	mPointsSpriteSheet           = new S2D::Texture2D();

	mExtraLivesSpriteSheet->Load("Textures/UI/LifeIcon.png", false);
	mPointsSpriteSheet->Load("Textures/UI/PointsSpriteSheet.png", false);

	// Check that the sprite sheets have been loaded in correctly
	if (!mExtraLivesSpriteSheet)
		std::cout << "Failed to load in the sprite sheet for the Lives UI icon." << std::endl;

	if (!mPointsSpriteSheet)
		std::cout << "Failed to load in the sprite sheet for the UI points." << std::endl;

	// Set the display positions
	mCollectedPickUpsPosition      = S2D::Vector2(400, 600);
	mExtraLivesStartTopRightPos    = S2D::Vector2(150, 604);
	mHighScoreDisplayPosition      = S2D::Vector2(HALF_SCREEN_WIDTH, 80);
	mCurrentScoreDisplayPosition   = S2D::Vector2(96, 80);

	// Setup the extra life render rect
	mExtraLifeRenderRect           = new S2D::Rect(0, 0, mExtraLivesSpriteSheet->GetWidth(), mExtraLivesSpriteSheet->GetHeight());

	mPointsSingleSpriteWidth       = mPointsSpriteSheet->GetWidth() / mAmountOfSpritesOnPointsSpriteSheetWidth;
	mPointsSingleSpriteHeight      = mPointsSpriteSheet->GetHeight() / mAmountOfSpritesOnPointsSpriteSheetHeight;
	mPointsSourceRect              = new S2D::Rect(0, 0, mPointsSingleSpriteWidth, mPointsSingleSpriteHeight);

	mTextRenderer                  = new TextRenderer("Textures/UI/Font.png", 15, 21);

	mDisplayingPointsToScreen      = false;
	mTimeRemainingForPointsDisplay = TIME_POINTS_SHOW_FOR;
}

// ---------------------------------------------------------------- //

UIManager::~UIManager()
{
	delete mExtraLivesSpriteSheet;
		mExtraLivesSpriteSheet = nullptr;

	delete mPointsSpriteSheet;
		mPointsSpriteSheet = nullptr;

	delete mExtraLifeRenderRect;
		mExtraLifeRenderRect = nullptr;

	delete mTextRenderer;
		mTextRenderer = nullptr;

	delete mPointsSourceRect;
		mPointsSourceRect = nullptr;

	delete mPointsDestRect;
		mPointsDestRect = nullptr;

	for(unsigned int i = 0; i < mCollectedPickups.size(); i++)
	{
		delete mCollectedPickups[i];
	}

	mCollectedPickups.clear();

	delete mInstance;
		mInstance = nullptr;
}

// ---------------------------------------------------------------- //

void UIManager::Update(const float deltaTime)
{
	if (mDisplayingPointsToScreen)
	{
		mTimeRemainingForPointsDisplay -= deltaTime;

		if (mTimeRemainingForPointsDisplay <= 0.0f)
			mDisplayingPointsToScreen = false;
	}
}

// ---------------------------------------------------------------- //

void UIManager::Render()
{
	RenderScores();

	RenderExtraLives();

	RenderCollectables();

	if(mDisplayingPointsToScreen)
		RenderPoints();
}

// ---------------------------------------------------------------- //

UIManager* UIManager::GetInstance()
{
	if (!mInstance)
		mInstance = new UIManager;

	return mInstance;
}

// ---------------------------------------------------------------- //

void UIManager::AddCollectedPickup(PICKUP_TYPES typeToAdd)
{
	mCollectedPickups.push_back(new PickUps(typeToAdd));
}

// ---------------------------------------------------------------- //

void UIManager::RenderScores()
{
	if (mTextRenderer)
	{
		// First render the text at the top
		mTextRenderer->Render("HIGH SCORE", 12, S2D::Vector2((QUATER_SCREEN_WIDTH * 3) - 48, 50), 7);

		// Now render the player's current score
		mTextRenderer->Render(to_string(GameManager::Instance()->GetCurrentScore()), 20, mCurrentScoreDisplayPosition, 0);

		// Now render the currently saved highscore
		mTextRenderer->Render(to_string(GameManager::Instance()->GetCurrentHighScore()), 20, mHighScoreDisplayPosition, 0);
	}
}

// ---------------------------------------------------------------- //

void UIManager::RenderCollectables()
{
	unsigned int startPoint = mCollectedPickups.size() < 7 ? 0 : mCollectedPickups.size() - 7;

	for (unsigned int i = startPoint; i < mCollectedPickups.size(); i++)
	{
		mCollectedPickups[i]->Render(mCollectedPickUpsPosition - S2D::Vector2(float(30 * (i - startPoint)), 0.0f));
	}
}

// ---------------------------------------------------------------- //

void UIManager::RenderExtraLives()
{
	if (mExtraLivesSpriteSheet && mExtraLifeRenderRect)
	{
		// Now render the extra lives icons
		for (unsigned int lifeID = 0; lifeID < GameManager::Instance()->GetExtraLivesCount(); lifeID++)
		{
			S2D::SpriteBatch::Draw( mExtraLivesSpriteSheet,
				                  &(mExtraLivesStartTopRightPos - S2D::Vector2(34.0f * lifeID, 0.0f)),
				                    mExtraLifeRenderRect);
		}
	}
}

// ---------------------------------------------------------------- //

void UIManager::DisplayPoints(S2D::Vector2 centrePosition, bool fromGhost, unsigned int magnitude)
{
	mDisplayingPointsToScreen      = true;
	mTimeRemainingForPointsDisplay = TIME_POINTS_SHOW_FOR;

	delete mPointsDestRect;
	mPointsDestRect = nullptr;

	delete mPointsSourceRect;
	mPointsSourceRect = nullptr;

	// Setup where the points are going to be rendered in the maze
	mPointsDestRect                = new S2D::Rect(centrePosition.X - (mPointsSingleSpriteWidth / 2),
												   centrePosition.Y - (mPointsSingleSpriteHeight / 2),
		                                           mPointsSingleSpriteWidth, 
		                                           mPointsSingleSpriteHeight);

	if(fromGhost)
		mPointsSourceRect              = new S2D::Rect(float(magnitude * mPointsSingleSpriteWidth), 
			                                           0.0f, 
			                                           mPointsSingleSpriteWidth, 
			                                           mPointsSingleSpriteHeight);
	else
		mPointsSourceRect              = new S2D::Rect(     float((magnitude % mAmountOfSpritesOnPointsSpriteSheetWidth)       * mPointsSingleSpriteWidth),
			                                            float(int(magnitude / mAmountOfSpritesOnPointsSpriteSheetWidth) + 1) * mPointsSingleSpriteHeight,
			                                            mPointsSingleSpriteWidth, 
			                                            mPointsSingleSpriteHeight);
}

// ---------------------------------------------------------------- //

void UIManager::RenderPoints()
{
	if(mPointsDestRect && mPointsSourceRect && mPointsSpriteSheet)
		S2D::SpriteBatch::Draw(mPointsSpriteSheet, mPointsDestRect, mPointsSourceRect);
}

// ---------------------------------------------------------------- //