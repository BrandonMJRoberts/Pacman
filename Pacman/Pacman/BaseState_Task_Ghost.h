#ifndef _BASE_STATE_TASK_GHOST_H_
#define _BASE_STATE_TASK_GHOST_H_

class Ghost;

class BaseState_Ghost abstract
{
public:
	BaseState_Ghost();
	virtual ~BaseState_Ghost();

	virtual void OnUpdate();
	virtual void CheckTransitions(Ghost* ghost);

protected:
	virtual void OnEnter();
	virtual void OnExit();

};

#endif