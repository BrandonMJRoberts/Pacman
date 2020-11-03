#ifndef _UI_MANAGER_H_
#define _UI_MANAGER_H_

#include "S2D/S2D.h"

#include "Commons.h"

class PickUps;
class TextRenderer;

class UIManager final
{
public:
	~UIManager();

	static UIManager* GetInstance();

	void Update(const float deltaTime);
	void Render();

	void AddCollectedPickup(PICKUP_TYPES typeToAdd);
	void DisplayPoints(S2D::Vector2 position, bool fromGhost, unsigned int magnitude);

private:
	UIManager();

	void                   RenderScores();
	void                   RenderCollectables();
	void                   RenderExtraLives();
	void                   RenderPoints();

	static UIManager*      mInstance;

	// BMP font renderer
	TextRenderer*          mTextRenderer;

	// Pickups
	std::vector<PickUps*>  mCollectedPickups;
	S2D::Vector2		   mCollectedPickUpsPosition;

	// Extra Lives
	S2D::Vector2	       mExtraLivesStartTopRightPos;
	S2D::Texture2D*		   mExtraLivesSpriteSheet;
	S2D::Rect*             mExtraLifeRenderRect;

	// In Maze Points
	S2D::Texture2D*		   mPointsSpriteSheet;
	S2D::Rect*             mPointsSourceRect;
	S2D::Rect*             mPointsDestRect;

	const unsigned int     mAmountOfSpritesOnPointsSpriteSheetHeight;
	const unsigned int     mAmountOfSpritesOnPointsSpriteSheetWidth;

	unsigned int           mPointsSingleSpriteWidth;
	unsigned int           mPointsSingleSpriteHeight;

	// Scores
	S2D::Vector2		   mHighScoreDisplayPosition;
	S2D::Vector2		   mCurrentScoreDisplayPosition;

	float                  mTimeRemainingForPointsDisplay;

	bool                   mDisplayingPointsToScreen;
};

#endif // !_UI_MANAGER_H_
