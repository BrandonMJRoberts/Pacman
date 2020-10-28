#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "Pickups.h"

class UIManager final
{
public:
	~UIManager();

	static UIManager* GetInstance();
	void Render();

	void AddCollectedPickup(PICKUP_TYPES typeToAdd);

private:
	UIManager();

	static UIManager* mInstance;

	std::vector<PickUps*>  mCollectedPickups;
	S2D::Vector2		   mCollectedPickUpsPosition;

	S2D::Vector2	       mExtraLivesStartTopRightPos;
	S2D::Texture2D*		   mExtraLivesSpriteSheet;
	S2D::Rect*             mExtraLifeRenderRect;

	S2D::Texture2D*		   mPointsSpriteSheet;

	S2D::Vector2		   mHighScoreDisplayPosition;
	S2D::Vector2		   mCurrentScoreDisplayPosition;

	S2D::Texture2D*        mCurrentScoreFontSpriteSheet;
};

#endif // !_UI_MANAGER_H_
