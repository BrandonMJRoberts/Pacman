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
	PacmanCharacter(const char** const collisionMap, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight, const S2D::Vector2 startPositon, const char* filePathForMainSpriteSheet, const char* filePathForAlternateSpriteSheet, bool isAIControlled);
	~PacmanCharacter() override;

	void             Update(const float deltaTime) override;
private:

	void             ReSetupPacmanSourceRect(float x, float y, unsigned int width, unsigned int height);
	void             CheckForDirectionChange() override;

	void             UpdateAsPlayerControlled(const float deltaTime);
	void             UpdateAsAI();

	Stack_FiniteStateMachine_Pacman* mStateMachine;
};

#endif

