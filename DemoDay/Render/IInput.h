#pragma once

#include "WorldTransformation.h"

class ModeFlag;

class IInput
{
protected:
	ModeFlag* modeFlag;

public:
	virtual void SetFlag(ModeFlag*) = 0;
	virtual void ModeChange() = 0;
};