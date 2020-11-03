#ifndef _MAIN_GAME_SCREEN_H_
#define _MAIN_GAME_SCREEN_H_

#include "BaseMenu.h"

#include "Commons.h"

class PickUps;
class PacmanCharacter;
class Background;
class DotsHandler;

class MainGameScreen : public BaseMenu
{
public:
	MainGameScreen();
	~MainGameScreen() override;

	void             Render(const unsigned int frameCount) override;
	SCREENS          Update(const float deltaTime)         override;

private:
	void             LoadInDataForLevel();
	void             SpawnNextCollectable();

	SCREENS          InGameInputCheck();

	PacmanCharacter* mPlayer;
	PickUps*         mCollectable;

	Background*      mBackground;
	DotsHandler*     mDotHandler;

	float            mTimeTillNextCollectableSpawn;
};


#endif