#ifndef _BASE_STATE_TASK_PACMAN_H_
#define _BASE_STATE_TASK_PACMAN_H_

class BaseState_Pacman
{
public:
	BaseState_Pacman();
	virtual ~BaseState_Pacman();

	virtual void OnUpdate();
	virtual void CheckTransitions();

protected:
	virtual void OnEnter();
	virtual void OnExit();
};

#endif