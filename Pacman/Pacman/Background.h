#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_

#include "S2D/S2D.h"

class Background
{
public:
	Background() = delete;
	Background(unsigned int backgroundColourIndex = 0, unsigned int spritesOnWidth = 14, unsigned int spritesOnHeight = 3);
	~Background();

	void Render();
	void Update();

	char**       GetCollisionMap()    { return mCollisionMap; }
	unsigned int GetCollisionWidth()  { return mWidth;  }
	unsigned int GetCollisionHeight() { return mHeight; }

	void         ChangeColourIndex(unsigned int newIndex);
	void         LoadInBackgroundSpriteMap();

private:
	void LoadInCollisionMap();

	char**          mCollisionMap;
	unsigned int** mBackgroundSpriteMap;

	S2D::Rect* mSpriteSheetSourceRenderRect;
	S2D::Rect* mSpriteSheetDestRenderRect;

	unsigned int mSingleTileWidth;
	unsigned int mSingleTileHeight;

	S2D::Texture2D* mTileSpriteSheet;

	unsigned int    mHeight;
	unsigned int    mWidth;

	unsigned int mAmountOfSpritesOnSpriteSheetWidth;
	unsigned int mAmountOfSpritesOnSpriteSheetHeight;

	unsigned int mColourIndex;

	bool mButtonCurrentlyPressed;
};

#endif