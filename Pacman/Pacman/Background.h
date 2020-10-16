#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "S2D/S2D.h"

class Background
{
public:
	Background();
	~Background();

	void Render();

	char**       GetCollisionMap()    { return mCollisionMap; }
	unsigned int GetCollisionWidth()  { return mWidth;  }
	unsigned int GetCollisionHeight() { return mHeight; }

private:
	void LoadInCollisionMap();

	char**          mCollisionMap;

	S2D::Texture2D* mBackgroundSprite;
	S2D::Rect*      mRenderRect;

	unsigned int    mHeight;
	unsigned int    mWidth;

};

#endif