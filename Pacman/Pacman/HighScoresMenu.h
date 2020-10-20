#ifndef _HIGHSCORES_MENU_H_
#define _HIGHSCORES_MENU_H_

#include "S2D/S2D.h"

class HighScoresMenu
{
public:
	HighScoresMenu();
	~HighScoresMenu();

	void            Render();
	bool            Update();

private:
	S2D::Texture2D* mBackgroundSprite;
	S2D::Rect*      mRenderRect;

	S2D::Vector2*   mHighScoresLabelPosition;
	S2D::Vector2*   mHighScoresTopOptionPosition;
};


#endif
