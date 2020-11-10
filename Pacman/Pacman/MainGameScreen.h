#ifndef _MAIN_GAME_SCREEN_H_
#define _MAIN_GAME_SCREEN_H_

#include "BaseMenu.h"

#include "Commons.h"

#include <vector>

class PickUps;
class PacmanCharacter;
class Background;
class DotsHandler;
class Ghost;

class MainGameScreen : public BaseMenu
{
public:
	MainGameScreen();
	~MainGameScreen() override;

	void             Render(const unsigned int frameCount) override;
	SCREENS          Update(const float deltaTime)         override;

private:
	void             LoadNextLevel();
	void             LoadInDataForLevel();
	void             SpawnNextCollectable();
	void             HandleCollectable(const float deltaTime);

	SCREENS          InGameInputCheck();

	PacmanCharacter*    mPlayer;
	PickUps*            mCollectable;

	Background*         mBackground;
	DotsHandler*        mDotHandler;
	std::vector<Ghost*> mGhosts;

	float               mTimeTillNextCollectableSpawn;
};


#endif