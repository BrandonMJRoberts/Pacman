#ifndef _PACMAN_CHARACTER_H_
#define _PACMAN_CHARACTER_H_

#include "S2D/S2D.h"

#include "Commons.h"

#include "BaseState_Task_Pacman.h"
#include "BaseCharacter.h"

class Stack_FiniteStateMachine_Pacman;
class DotsHandler;

class PacmanCharacter final : public BaseCharacter
{
public:
	PacmanCharacter() = delete;
	PacmanCharacter(char**       const collisionMap, 
		            const unsigned int spritesOnWidthMain, 
		            const unsigned int spritesOnHeightMain, 
					const unsigned int spritesOnWidthAlternate,
					const unsigned int spritesOnHeightAlternate,
		            const S2D::Vector2 startPositon, 
		            const char*        filePathForMainSpriteSheet, 
		            const char*        filePathForAlternateSpriteSheet, 
		            bool               isAIControlled);
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

