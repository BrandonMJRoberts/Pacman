#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_

#include "S2D/S2D.h"

#include "BaseMenu.h"

class PauseMenu final : public BaseMenu
{
public:
	PauseMenu();
	~PauseMenu();

	void            Render();

	// Cannot override the parent function as it has a different return type and parameters
	// But this will be called instead of the parent's
	SCREENS         Update(const float deltaTime);

private:
	S2D::Texture2D* mBackgroundSprite;
	S2D::Rect*      mRenderRectangle;
	S2D::Vector2*   mStringPosition;
};

#endif // !_PAUSE_MENU_H_

