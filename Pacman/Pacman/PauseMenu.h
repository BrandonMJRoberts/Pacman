#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_

#include "S2D/S2D.h"

#include "BaseMenu.h"

class TextRenderer;

class PauseMenu final : public BaseMenu
{
public:
	PauseMenu();
	~PauseMenu() override;

	void            Render(unsigned int frameCount) override;

	// Cannot override the parent function as it has a different return type and parameters
	// But this will be called instead of the parent's
	SCREENS         Update(const float deltaTime) override;

private:
	S2D::Texture2D* mBackgroundSprite;
	S2D::Rect*      mRenderRectangle;

	TextRenderer*   mTextRenderer;

	S2D::Vector2    mPauseMenuTextPosition;
	S2D::Vector2    mCurrentScoreTextPosition;
	S2D::Vector2    mHighScoreTextPosition;
	S2D::Vector2    mRemainingLivesCountTextPosition;
	S2D::Vector2    mPlayerNameTextPosition;
	S2D::Vector2    mGhostsEatenThisGameTextPosition;
	S2D::Vector2    mDotsCollectedThisGameCountTextPosition;
};

#endif // !_PAUSE_MENU_H_

