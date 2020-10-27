#ifndef _PICKUPS_H_
#define _PICKUPS_H_

#include "Commons.h"
#include "Constants.h"

#include "S2D/S2D.h"

class PickUps final
{
public:
	PickUps();
	PickUps(const PICKUP_TYPES type = PICKUP_TYPES::APPLE, const S2D::Vector2 spawnPosition = S2D::Vector2(14 * SPRITE_RESOLUTION, 17.5 * SPRITE_RESOLUTION));
	~PickUps();

	void Render();
	void Render(const S2D::Vector2 positionToRender);

	bool CheckForCollision(const S2D::Vector2 pacmanPosition, const unsigned int pacmanDimensions, const FACING_DIRECTION pacmanFacingDirection);

	PICKUP_TYPES GetType() { return mThisPickupType; }

private:
	void LoadSpriteSheet();
	void SetupRenderData();

	PICKUP_TYPES    mThisPickupType;

	S2D::Vector2    mCentrePosition;

	unsigned int    mSingleSpriteWidth;
	unsigned int    mSingleSpriteHeight;

	S2D::Texture2D* mSpriteSheet;
	S2D::Rect*      mSourceRect;

	bool            mCollidedWith;
	bool            mCollidedFromLeft;
};

#endif // !_PICKUPS_H_
