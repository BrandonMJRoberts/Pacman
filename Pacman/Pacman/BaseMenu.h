#ifndef _BASE_MENU_H_
#define _BASE_MENU_H_

#include "Commons.h"

class BaseMenu
{
public:
	BaseMenu();
	~BaseMenu();

	virtual void    Render(const unsigned int frameCount);
	virtual SCREENS Update(const float deltaTime);

};

#endif // !_BASE_MENU_H_
