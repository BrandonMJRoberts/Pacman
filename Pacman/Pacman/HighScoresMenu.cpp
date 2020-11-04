#include "HighScoresMenu.h"

#include "Constants.h"

#include "TextRenderer.h"

#include <iostream>
#include <fstream>
#include <sstream>

// ---------------------------------------------------------------- //

HighScoresMenu::HighScoresMenu() : BaseMenu()
{
	// Setup the text renderer
	mTextRenderer                 = new TextRenderer("Textures/UI/Font.png", 15, 21);

	mHighScoresTextRenderPosition = S2D::Vector2(330, 50);
	mScoresRenderOffset           = S2D::Vector2(330, 100);
	mNamesRenderOffset            = S2D::Vector2(180, 100);

	mGapBetweenScores             = 50.0f;

	// Load in the scores
	LoadInScores();
}

// ---------------------------------------------------------------- //

HighScoresMenu::~HighScoresMenu()
{
	delete mTextRenderer;
	mTextRenderer = nullptr;

	mScores.clear();
}

// ---------------------------------------------------------------- //

void HighScoresMenu::Render(unsigned int frameCount)
{
	// First render the words 'HIGH SCORES:'
	mTextRenderer->Render("HIGH SCORES", 15, mHighScoresTextRenderPosition, 0);

	// Loop through all the scores and render them
	for (unsigned int i = 0; i < 10; i++)
	{
		if (i < mScores.size())
		{
			// Render the name first, then the score
			mTextRenderer->Render(mScores[i].name, 10, mNamesRenderOffset + S2D::Vector2(0.0f, mGapBetweenScores * i), mScores[i].colourIndex);

			mTextRenderer->Render(to_string(mScores[i].score), 100, mScoresRenderOffset + S2D::Vector2(0.0f, mGapBetweenScores * i), mScores[i].colourIndex);
		}
		else
		{
			// Render a preset for filler space
			mTextRenderer->Render("FILLER", 10, mNamesRenderOffset + S2D::Vector2(0.0f, mGapBetweenScores * i), (i % 7));

			mTextRenderer->Render("10", 10, mScoresRenderOffset + S2D::Vector2(0.0f, mGapBetweenScores * i), (i % 7));
		}
	}
}

// ---------------------------------------------------------------- //

void HighScoresMenu::LoadInScores()
{
	std::ifstream file("Save Data/HighScores.txt");

	if (file.is_open())
	{
		char*             cLine = new char[100];
		std::string       sLine, name;
		unsigned int      score, colourIndex;
		std::stringstream ssLine;

		while (file.getline(cLine, 100))
		{
			// Read in the data
			sLine  = cLine;
			ssLine = std::stringstream(sLine);

			ssLine >> name >> score >> colourIndex;

			mScores.push_back(ScoreData(score, name, colourIndex));
		}

		delete[] cLine;
	}
	else
	{
		std::cout << "Failed to load in the highscores file." << std::endl;
	}
}

// ---------------------------------------------------------------- //

ScoreData::ScoreData(unsigned int startScore, std::string startName, unsigned int startColourIndex)
{
	score       = startScore;
	name        = startName;
	colourIndex = startColourIndex;
}

// ---------------------------------------------------------------- //