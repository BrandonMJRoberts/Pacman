#ifndef _PACMAN_CHARACTER_H_
#define _PACMAN_CHARACTER_H_

#include "S2D/S2D.h"

#include "Commons.h"

#include "BaseState_Task_Pacman.h"
#include "BaseCharacter.h"

class Stack_FiniteStateMachine_Pacman;
class DotsHandler;

// --------------------------------------------------------------------------------------------------------------------------------------------- //

struct PacmanCreationData
{
	PacmanCreationData(char** const       collisionMap,
					   const unsigned int spritesOnWidthMain,
					   const unsigned int spritesOnHeightMain,
					   const unsigned int spritesOnWidthAlternate,
					   const unsigned int spritesOnHeightAlternate,
					   const S2D::Vector2 startPositon,
				       const char*        filePathForMainSpriteSheet,
					   const char*        filePathForAlternateSpriteSheet) : mCollisionMap(collisionMap), 
		                                                                     mSpritesOnWidthMain(spritesOnWidthMain), 
		                                                                     mSpritesOnHeightMain(spritesOnHeightMain),
																			 mSpritesOnWidthAlternate(spritesOnWidthAlternate),
																		     mSpritesOnHeightAlternate(spritesOnHeightAlternate),
																			 mStartPositon(startPositon),
		                                                                     mFilePathForMainSpriteSheet(filePathForMainSpriteSheet),
																		     mFilePathForAlternateSpriteSheet(filePathForAlternateSpriteSheet)
	{	}

	~PacmanCreationData() { ; }

	char** const       mCollisionMap;
	const unsigned int mSpritesOnWidthMain;
	const unsigned int mSpritesOnHeightMain;
	const unsigned int mSpritesOnWidthAlternate;
	const unsigned int mSpritesOnHeightAlternate;
	const S2D::Vector2 mStartPositon;
	const char*        mFilePathForMainSpriteSheet;
	const char*        mFilePathForAlternateSpriteSheet;
};

// --------------------------------------------------------------------------------------------------------------------------------------------- //

class PacmanCharacter final : public BaseCharacter
{
public:
	PacmanCharacter() = delete;
	PacmanCharacter(PacmanCreationData, bool isAIControlled);
	~PacmanCharacter() override;

	//void             Update(const float deltaTime) override;
	void             Update(const float deltaTime, std::vector<S2D::Vector2> ghostPositions, DotsHandler& dotManager);
	void             SetIsAlive(bool newVal) override;

	void             Render(const unsigned int frameCount) override;

	Stack_FiniteStateMachine_Pacman& GetStateMachine() { return *mStateMachine; }

private:

	void             ReSetupPacmanSourceRect(float x, float y, unsigned int width, unsigned int height);
	void             CheckForDirectionChange() override;

	void             ResetPacmanFromDeath();

	Stack_FiniteStateMachine_Pacman* mStateMachine;
};

#endif

