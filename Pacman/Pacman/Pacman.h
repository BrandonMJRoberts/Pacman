#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

#include "PacmanCharacter.h"
#include "Background.h"
#include "DotsHandler.h"

#include "PauseMenu.h"
#include "StartMenu.h"
#include "HighScoresMenu.h"

#include "Pickups.h"

#include "TextRenderer.h"

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

private:
	void             StartMenuUpdate(const float deltaTime);
	void             PauseMenuUpdate(const float deltaTime);
	void             HighScoreMenuUpdate();
	void             InGameUpdate(const float deltaTime);
	void             InGameInputCheck();
	
	void             MainGameRender();

	void             LoadInDataForLevel();

	void             SpawnNextCollectable();

	PacmanCharacter* mPlayer;
	PickUps*         mCollectable;

	PauseMenu*       mPauseMenu;
	StartMenu*       mStartMenu;
	HighScoresMenu*  mHighScoreMenu;

	Background*      mBackground;
	DotsHandler*     mDotHandler;
	TextRenderer*    mTextRenderer;

	int              _frameCount;

	float            mTimeTillNextCollectableSpawn;

	bool             mInStartMenu;
	bool             mInHighscoreMenu;
	bool             mInMainGame;
};