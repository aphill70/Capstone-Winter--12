#pragma once

#include "IInput.h"
#include "MouseInput.h"
#include "KeyboardInput.h"
#include "HapticInput.h"

class InputManager
{
private:
	MouseInput mouseInput;
	KeyboardInput keyboardInput;
	HapticInput hapticInput;
	ModeFlag mode;

public:	
	InputManager(){}


};