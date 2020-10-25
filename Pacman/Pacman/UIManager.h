#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "Pickups.h"

class UIManager final
{
public:
	UIManager();
	~UIManager();

	void Render();

private:
	PickUps*        mCollectedPickups;
	S2D::Vector2    mCollectedPickUpsPosition;

	S2D::Vector2    mExtraLivesTopRight;
	S2D::Texture2D* mExtraLivesSpriteSheet;

	S2D::Texture2D* mPointsSpriteSheet;

	S2D::Vector2    mHighScoreDisplayPosition;
	S2D::Vector2    mCurrentScoreDisplayPosition;


};

#endif // !_UI_MANAGER_H_
