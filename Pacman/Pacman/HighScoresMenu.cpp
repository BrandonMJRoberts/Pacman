#include "HighScoresMenu.h"

#include "Constants.h"

#include <iostream>
#include <fstream>
#include <sstream>

// ---------------------------------------------------------------- //

HighScoresMenu::HighScoresMenu()
{
	// Setup the text renderer
	mTextRenderer = new TextRenderer("Textures/UI/Font.png", 15, 21);

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

void HighScoresMenu::Render()
{
	// First render the words 'HIGH SCORES:'
	mTextRenderer->Render("HIGH SCORES", 15, S2D::Vector2(300,50), 0);

	// Loop through all the scores and render them
	for (unsigned int i = 0; i < 10; i++)
	{
		if (i < mScores.size())
		{
			// Render the name first, then the score
			mTextRenderer->Render(mScores[i].GetName(), 10, S2D::Vector2(150, 100 + (i * 50)), mScores[i].GetColourIndex());

			mTextRenderer->Render(to_string(mScores[i].GetScore()), 100, S2D::Vector2(300, 100 + (i * 50)), mScores[i].GetColourIndex());
		}
		else
		{
			// Render a preset for filler space
			mTextRenderer->Render("FILLER", 10, S2D::Vector2(150, 100 + (i * 50)), (i % 7));

			mTextRenderer->Render("10", 100, S2D::Vector2(300, 100 + (i * 50)), (i % 7));
		}
	}
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
			sLine = cLine;
			ssLine = std::stringstream(sLine);

			ssLine >> name >> score >> colourIndex;

			mScores.push_back(ScoreData(score, name, colourIndex));
		}

		delete cLine;
	}
	else
	{
		std::cout << "Failed to load in the highscores file." << std::endl;
	}
}

// ---------------------------------------------------------------- //

ScoreData::ScoreData(unsigned int score, std::string name, unsigned int colourIndex)
{
	mScore       = score;
	mName        = name;
	mColourIndex = colourIndex;
}

// ---------------------------------------------------------------- //