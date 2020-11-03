#ifndef _HIGHSCORES_MENU_H_
#define _HIGHSCORES_MENU_H_

#include "S2D/S2D.h"

#include "BaseMenu.h"

#include <string>

class TextRenderer;

// -------------------------------------------------------------------------------------------------- //

struct ScoreData
{
public:
	ScoreData(unsigned int score, std::string name, unsigned int colourIndex);
	~ScoreData() { ; }

	std::string  name;
	unsigned int score;
	unsigned int colourIndex;
};

// -------------------------------------------------------------------------------------------------- //

class HighScoresMenu final : public BaseMenu
{
public:
	HighScoresMenu();
	~HighScoresMenu();

	void            Render(unsigned int frameCount) override;

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
