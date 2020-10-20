#ifndef _START_MENU_H_
#define _START_MENU_H_

#include "S2D/S2D.h"
#include "Commons.h"

// ------------------------------------------------------------------------- //

class StartMenu
{
public:
	StartMenu();
	~StartMenu();

	void              Render(unsigned int frameCount);
	SELECTION_OPTIONS Update(const float deltaTime);

private:
	void              HandleDownPress(S2D::Input::KeyboardState* keyboardState);
	void              HandleUpPress(S2D::Input::KeyboardState* keyboardState);
	SELECTION_OPTIONS HandleReturnPress(S2D::Input::KeyboardState* keyboardState);

	S2D::Texture2D*   mBackgroundSprite;
	S2D::Rect*        mBackgroundRenderRect;

	S2D::Texture2D*   mSelectorSprite;
	S2D::Rect*        mSelectorRenderRect;

	S2D::Vector2*     mStartOptionPosition;
	S2D::Vector2*     mHighScoresOptionPosition;
	S2D::Vector2*     mQuitOptionPosition;
	S2D::Vector2*     mStartMenuLabelPosition;
	S2D::Vector2*     mChangeCharacterOptionPosition;
	S2D::Vector2*     mSelectorPosition;

	unsigned int      mAmountOfSpitesOnSelectorHeight;
	unsigned int      mAmountOfSpitesOnSelectorWidth;

	SELECTION_OPTIONS mCurrentlySelectedOption;
	bool              mButtonCurrentlyPressed;
};

// ------------------------------------------------------------------------- //

#endif // !_START_MENU_H_
