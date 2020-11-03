#include "BaseMenu.h"

#include "S2D/S2D.h"

// ----------------------------------------------- //

BaseMenu::BaseMenu()
{

}

// ----------------------------------------------- //

BaseMenu::~BaseMenu()
{

}

// ----------------------------------------------- //

void BaseMenu::Render(const unsigned int frameCount)
{

}

// ----------------------------------------------- //

SCREENS BaseMenu::Update(const float deltaTime)
{
	// Allow the player to return back to the previous screen
	if (S2D::Input::Keyboard::GetState()->IsKeyDown(S2D::Input::Keys::ESCAPE))
	{
		return SCREENS::MAIN_MENU;
	}

	return SCREENS::SAME;
}

// ----------------------------------------------- //