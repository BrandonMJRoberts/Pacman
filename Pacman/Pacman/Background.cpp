#include "Background.h"

#include "GameManager.h"
#include "S2D/S2D.h"
#include "Constants.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// ----------------------------------------------------------- //

Background::Background()
{
	mBackgroundSprite = new S2D::Texture2D();
	std::string filePath = "Textures/Backgrounds/Background";
	filePath.append(to_string(GameManager::Instance()->GetCurrentLevel()));
	filePath.append(".png");

	mBackgroundSprite->Load(filePath.c_str(), false);
	if (!mBackgroundSprite)
	{
		std::cout << "Failed to load the background sprite: " << GameManager::Instance()->GetCurrentLevel() << std::endl;
	}

	mRenderRect = new S2D::Rect(GameManager::Instance()->GetGridOffset().X, 
		                        GameManager::Instance()->GetGridOffset().Y, 
		                        mBackgroundSprite->GetWidth(), 
		                        mBackgroundSprite->GetHeight());

	LoadInCollisionMap();
}

// ----------------------------------------------------------- //

Background::~Background()
{
	delete mBackgroundSprite;
	mBackgroundSprite = nullptr;
}

// ----------------------------------------------------------- //

void Background::Render()
{
	S2D::SpriteBatch::Draw(mBackgroundSprite, mRenderRect);
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

	char* charLine = new char[100];
	std::string sLine = "";
	std::stringstream ssLine;
	
	unsigned int currentRow = 0;

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
			mCollisionMap = new char* [mWidth];

			for (unsigned int i = 0; i < mWidth; i++)
			{
				mCollisionMap[i] = new char[mHeight];
			}
		}

		for (unsigned int charID = 0; charID < sLine.size(); charID++)
		{
			mCollisionMap[charID][currentRow] = sLine[charID];
		}

		currentRow++;
		continue;
	}
}

// ----------------------------------------------------------- //