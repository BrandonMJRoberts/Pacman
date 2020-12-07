#ifndef _PACMAN_CHARACTER_H_
#define _PACMAN_CHARACTER_H_

#include "S2D/S2D.h"

#include "Commons.h"

#include "BaseState_Task_Pacman.h"
#include "BaseCharacter.h"

class Stack_FiniteStateMachine_Pacman;

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

	void             Update(const float deltaTime) override;
	void             SetIsAlive(bool newVal) override;

	void             Render(const unsigned int frameCount) override;
private:

	void             ReSetupPacmanSourceRect(float x, float y, unsigned int width, unsigned int height);
	void             CheckForDirectionChange() override;

	void             UpdateAsPlayerControlled(const float deltaTime);
	void             UpdateAsAI();

	void             ResetPacmanFromDeath();

	Stack_FiniteStateMachine_Pacman* mStateMachine;

	bool                             mDeathAnimationIsComplete;
};

#endif

