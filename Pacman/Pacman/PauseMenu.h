#ifndef _PAUSE_MENU_H_
#define _PAUSE_MENU_H_

#include "S2D/S2D.h"

class PauseMenu final
{
public:
	PauseMenu();
	~PauseMenu();

	void            Render();
	bool            Update(const float deltaTime);

private:
	S2D::Texture2D* mBackgroundSprite;
	S2D::Rect*      mRenderRectangle;
	S2D::Vector2*   mStringPosition;
};

#endif // !_PAUSE_MENU_H_

