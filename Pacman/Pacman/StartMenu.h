#ifndef _START_MENU_H_
#define _START_MENU_H_

#include "S2D/S2D.h"
#include "Commons.h"

#include "BaseMenu.h"

class TextRenderer;

// ------------------------------------------------------------------------- //

class StartMenu final : public BaseMenu
{
public:
	StartMenu();
	~StartMenu() override;

	void    Render(const unsigned int frameCount);
	SCREENS Update(const float deltaTime);

private:
	void               HandleDownPress  (S2D::Input::KeyboardState& keyboardState);
	void               HandleUpPress    (S2D::Input::KeyboardState& keyboardState);
	SELECTION_OPTIONS  HandleReturnPress(S2D::Input::KeyboardState& keyboardState);

	void               TypingInName();

	const unsigned int       mAmountOfSpitesOnSelectorHeight;
	const unsigned int       mAmountOfSpitesOnSelectorWidth;

	S2D::Vector2       mStartGamePosition;
	S2D::Vector2       mHighScoresPosition;
	S2D::Vector2       mChangeCharacterPosition;
	S2D::Vector2       mExitGamePosition;
	S2D::Vector2       mSetNamePosition;

	S2D::Texture2D*    mSelectorSpriteSheet;
	S2D::Rect          mSelectorSourceRenderRect;
	S2D::Vector2       mSelectorPosition;

	unsigned int       mSelectorSpriteCurrentFrame;
	unsigned int       mSelectorSpriteStartFrame;
	unsigned int       mSelectorSpriteEndFrame;

	const unsigned int mAmountOfRenderedFramesPerAnimationFrame;

	float              mSingleSpriteWidth;
	float              mSingleSpriteHeight;

	TextRenderer*      mTextRenderer;

	SELECTION_OPTIONS  mCurrentlySelectedOption;
	bool               mButtonCurrentlyPressed;
	bool               mLettersPressedCurrently;
};

// ------------------------------------------------------------------------- //

#endif // !_START_MENU_H_
