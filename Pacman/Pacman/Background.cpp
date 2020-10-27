#include "Background.h"

#include "GameManager.h"
#include "S2D/S2D.h"
#include "Constants.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// ----------------------------------------------------------- //

Background::Background(const unsigned int spritesOnWidth, const unsigned int spritesOnHeight)
{
	mSpriteSheetDestRenderRect          = new S2D::Rect(0, 0, 0, 0);
	mSpriteSheetSourceRenderRect        = new S2D::Rect(0, 0, 0, 0);

	mAmountOfSpritesOnSpriteSheetWidth  = spritesOnWidth;
	mAmountOfSpritesOnSpriteSheetHeight = spritesOnHeight;

	mButtonCurrentlyPressed              = false;

	// First setup the background sprites sheet
	ChangeColourIndex(0);

	LoadInBackgroundSpriteMap();
	LoadInCollisionMap();
}

// ----------------------------------------------------------- //

Background::~Background()
{
	delete mTileSpriteSheet;
		mTileSpriteSheet = nullptr;

	delete mSpriteSheetSourceRenderRect;
		mSpriteSheetSourceRenderRect = nullptr;

	delete mSpriteSheetDestRenderRect;
		mSpriteSheetDestRenderRect = nullptr;

	for (unsigned int row = 0; row < mHeight; row++)
	{
		delete[] mBackgroundSpriteMap;
		delete[] mCollisionMap;
	}

	delete mBackgroundSpriteMap;
		mBackgroundSpriteMap = nullptr;

	delete mCollisionMap;
		mCollisionMap = nullptr;

}

// ----------------------------------------------------------- //

void Background::Render()
{
	mSpriteSheetDestRenderRect->Y = GameManager::Instance()->GetGridOffset().Y;
	mSpriteSheetDestRenderRect->X = GameManager::Instance()->GetGridOffset().X;

	// Loop through the entire grid and render
	for (unsigned int row = 0; row < mHeight; row++)
	{
		for (unsigned int col = 0; col < mWidth; col++)
		{
			mSpriteSheetSourceRenderRect->X =    float((mBackgroundSpriteMap[row][col] % mAmountOfSpritesOnSpriteSheetWidth) * SPRITE_RESOLUTION);
			mSpriteSheetSourceRenderRect->Y = float(int(mBackgroundSpriteMap[row][col] / mAmountOfSpritesOnSpriteSheetWidth) * SPRITE_RESOLUTION);

			S2D::SpriteBatch::Draw(mTileSpriteSheet, mSpriteSheetDestRenderRect, mSpriteSheetSourceRenderRect);

			mSpriteSheetDestRenderRect->X += mSingleTileWidth;
		}

		mSpriteSheetDestRenderRect->Y += mSingleTileHeight;
		mSpriteSheetDestRenderRect->X = 0;
	}
}

// ----------------------------------------------------------- //

void Background::LoadInCollisionMap()
{
	std::ifstream file;
	file.open("CollisionMaps/" + to_string(GameManager::Instance()->GetCurrentLevel()) + ".txt");
	if (!file.is_open())
	{
		std::cout << "Failed to open the collision file for this level" << std::endl;
		return;
	}

	char*             charLine = new char[100];
	std::string       sLine    = "";
	std::stringstream ssLine;
	
	unsigned int      currentRow = 0;

	// Now loop through all the lines in the file and get the data required
	while (sLine != "END")
	{
		file.getline(charLine, 100);

		sLine = charLine;

		// First we need to get the width and height of the data
		if (mWidth == 0 || mHeight == 0)
		{
			ssLine = std::stringstream(sLine);
			ssLine >> mWidth >> mHeight;
			continue;
		}

		if (!mCollisionMap)
		{
			mCollisionMap = new char* [mHeight];

			for (unsigned int i = 0; i < mHeight; i++)
			{
				mCollisionMap[i] = new char[mWidth];
			}
		}

		if (mCollisionMap && currentRow < mHeight)
		{
			for (unsigned int charID = 0; charID < sLine.size(); charID++)
			{
				mCollisionMap[currentRow][charID] = sLine[charID];
			}

			currentRow++;
			continue;
		}
	}
}

// ----------------------------------------------------------- //

void Background::ChangeColourIndex(const unsigned int newIndex)
{
	delete mTileSpriteSheet;

	mTileSpriteSheet = new S2D::Texture2D();
	mTileSpriteSheet->Load(std::string("Textures/Backgrounds/MazeParts" + to_string(newIndex) + ".png").c_str(), false);

	if (!mTileSpriteSheet)
	{
		std::cout << "Failed to load in the sprite sheet for the background sprites" << std::endl;
		return;
	}
}

// ----------------------------------------------------------- //

void Background::LoadInBackgroundSpriteMap()
{
	// First open the correct file
	std::ifstream file("BackgroundMaps/BackgroundMap.txt");

	if (!file.is_open())
	{
		std::cout << "Failed to open the background sprite map." << std::endl;
		return;
	}

	// Now we need to load in the data stored in the map
	char* cLine        = new char[200];

	unsigned int width = 0, height = 0, currentRow = 0;
	std::string stringVersionOfNumber;

	while (file.getline(cLine, 200))
	{
		// The first two lines are the width and height so read them in first
		if (width == 0 || height == 0)
		{
			std::stringstream ssLine(cLine);
			ssLine >> width >> height;

			mSingleTileHeight = mTileSpriteSheet->GetHeight() / mAmountOfSpritesOnSpriteSheetHeight;
			mSingleTileWidth  = mTileSpriteSheet->GetWidth()  / mAmountOfSpritesOnSpriteSheetWidth;

			mSpriteSheetSourceRenderRect->Width  = mSingleTileWidth;
			mSpriteSheetSourceRenderRect->Height = mSingleTileHeight;

			mSpriteSheetDestRenderRect->Width  = mSingleTileWidth;
			mSpriteSheetDestRenderRect->Height = mSingleTileHeight;

			continue;
		}

		if (!mBackgroundSpriteMap)
		{
			// Now we need to allocate the correct amount of memory required
			mBackgroundSpriteMap = new unsigned int* [height];

			for (unsigned int i = 0; i < height; i++)
			{
				mBackgroundSpriteMap[i] = new unsigned int[width];
			}
		}

		// Now we need to load in the actual data
		if (mBackgroundSpriteMap && currentRow < height)
		{
			std::stringstream ssLine(cLine);
			for (unsigned int i = 0; i < width; i++)
			{
				ssLine >> stringVersionOfNumber;
				std::stringstream(stringVersionOfNumber) >> mBackgroundSpriteMap[currentRow][i];
			}

			currentRow++;
			continue;
		}
	}
}

// ----------------------------------------------------------- //

void Background::Update()
{
	if (mColourIndex != GameManager::Instance()->GetCurrentLevel())
	{
		mColourIndex = GameManager::Instance()->GetCurrentLevel();
		ChangeColourIndex(mColourIndex);
	}
}

// ----------------------------------------------------------- //