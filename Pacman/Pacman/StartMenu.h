#ifndef _START_MENU_H_
#define _START_MENU_H_

#include "S2D/S2D.h"

// ------------------------------------------------------------------------- //

enum class SELECTION_OPTIONS : char
{
	START_GAME = 0x00,
	HIGHSCORES = 0x01,
	QUIT       = 0x02,
	NONE       = 0x04
};

// ------------------------------------------------------------------------- //

class StartMenu
{
public:
	StartMenu();
	~StartMenu();

	void Render();
	SELECTION_OPTIONS Update(const float deltaTime);

private:
	S2D::Texture2D*   mBackgroundSprite;
	S2D::Rect*        mBackgroundRenderRect;

	S2D::Texture2D*   mSelectorSprite;
	S2D::Rect*        mSelectorRenderRect;

	S2D::Vector2*     mStartOptionPosition;
	S2D::Vector2*     mHighScoresOptionPosition;
	S2D::Vector2*     mQuitOptionPosition;

	S2D::Vector2*     mStartMenuLabelPosition;
	S2D::Vector2*     mSelectorPosition;

	SELECTION_OPTIONS mCurrentlySelectedOption;
	bool              mButtonCurrentlyPressed;
};

// ------------------------------------------------------------------------- //

#endif // !_START_MENU_H_
