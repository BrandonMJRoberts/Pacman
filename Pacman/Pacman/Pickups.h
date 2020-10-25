#ifndef _PICKUPS_H_
#define _PICKUPS_H_

#include "Commons.h"

#include "S2D/S2D.h"

class PickUps final
{
public:
	PickUps();
	PickUps(PICKUP_TYPES type, S2D::Vector2 spawnPosition);
	~PickUps();

	void Render();

private:
	PICKUP_TYPES    mThisPickupType;

	S2D::Vector2    mPosition;

	S2D::Texture2D* mSpriteSheet;
	S2D::Rect*      mSourceRect;
	S2D::Rect*      mDestRect;
};

#endif // !_PICKUPS_H_
