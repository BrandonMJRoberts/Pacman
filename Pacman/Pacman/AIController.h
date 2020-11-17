#ifndef _AI_CONTROLLER_H_
#define _AI_CONTROLLER_H_

#include <vector>

class Stack_FiniteStateMachine_Ghost;
class Stack_FiniteStateMachine_Pacman;

class Ghost;
class PacmanCharacter;

class AIController final
{
public:
	AIController();
	AIController(char** collisionMapOfLevel);
	~AIController();

	void Update(const float deltaTime);
	void Render(const unsigned int frameCount);

	PacmanCharacter& GetPacmanRef()              { return *mPacman; }

private:
	void SetupGhosts(char** collisionMapOfLevel);
	void SetupPacman(char** collisionMap);

	std::vector<Stack_FiniteStateMachine_Ghost*>  mGhostStateMachines; 	   // One state machine per AI ghost character
	std::vector<Ghost*>                           mGhosts;                 // Vector of the ghosts in the level

	// State machine for pacman
	Stack_FiniteStateMachine_Pacman*              mPacmanStateMachine;     // Finite state machine for pacman
	PacmanCharacter*                              mPacman;                 // Pacman instance

	bool                                          mPacmanIsAI;
};

#endif

/*
	How this works is that there is a stack of states that the ghosts (and pacman) reference and obay
	The stack is a vector of the base state class.
	Each state class has an enum variable that holds what the state's name is, stored as an enum

	You can add to the top of the stack and pop off from the stack, as well as peeking the top most element. So you can make decisions based off of what state it is.

	The point of doing a stack system is so that more important states will not completly override prior states, they are only taking presidence currently.
	Once the state is completed it is popped off and the prior state continues.

	This is ideal for the ghosts when compared to just a finite state machine is that if they are chacing pacman and then pacman picks up a power pill. 
	They should then flee pacman until the time runs out, when they drop back down to the prior state of chasing pacman.

	//-----------//      //-----------//       //-----------//
	//===CHASE===// -->  //====FLEE===//  -->  //===CHASE===// 
	//-----------//      //-----------//       //-----------//

						 //-----------//
	  					 //===CHASE===//
						 //-----------//


	Instead of going: chase -> flee -> none
	you could fix this by storing the prior state, but then if you went two other states deep it would lose the first one, so you would need another vector of prior states. 
	Which this method does more efficiently.
*/



