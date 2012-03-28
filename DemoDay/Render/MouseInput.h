#pragma once

#include "IInput.h"
#include "ModeFlag.h"

class MouseInput : public IInput
{
public:
	MouseInput(void);

	virtual void ModeChange();
};