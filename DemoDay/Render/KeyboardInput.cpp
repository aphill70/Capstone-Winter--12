#include "KeyboardInput.h"

KeyboardInput::KeyboardInput(void) : 
	forwardPressed(false), backPressed(false), 
	leftPressed(false), rightPressed(false), 
	upPressed(false), downPressed(false),
	resetPressed(false) { }

void KeyboardInput::SetFlag(ModeFlag * flag)
{
	modeFlag = flag;
	modeFlag->AddListener(this);
}

void KeyboardInput::ModeChange()
{

}