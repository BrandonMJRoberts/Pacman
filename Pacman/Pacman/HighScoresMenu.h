#ifndef _HIGHSCORES_MENU_H_
#define _HIGHSCORES_MENU_H_

#include "S2D/S2D.h"

#include "TextRenderer.h"
#include "Commons.h"

#include <vector>

// -------------------------------------------------------------------------------------------------- //

struct ScoreData
{
public:
	ScoreData(unsigned int score, std::string name, unsigned int colourIndex);
	~ScoreData() { ; }

	unsigned int GetScore() { return mScore; }
	std::string GetName() { return mName; }
	unsigned int GetColourIndex() { return mColourIndex; }

private:
	std::string  mName;
	unsigned int mScore;
	unsigned int mColourIndex;
};

// -------------------------------------------------------------------------------------------------- //

class HighScoresMenu final
{
public:
	HighScoresMenu();
	~HighScoresMenu();

	void            Render();
	bool            Update();

private:
	void LoadInScores();

	TextRenderer*        mTextRenderer;
	vector<ScoreData>    mScores;

	S2D::Vector2         mHighScoresTextRenderPosition;
	S2D::Vector2         mScoresRenderOffset;
	S2D::Vector2         mNamesRenderOffset;

	float                mGapBetweenScores;

};


#endif
