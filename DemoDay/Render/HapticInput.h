#pragma once

#include "chai3d.h"
#include "glm/glm.hpp"
#include "IInput.h"
#include "ModeFlag.h"
#include <iostream>
#include "IHapticDevice.h"

#define BOX_RADIUS		0.75
#define MIN_DIST		3

using namespace glm;
using namespace std;

extern IHapticDevice* hapticDevice;

class HapticInput : public IInput
{
private:
	cGenericObject* cursor;

public:
	HapticInput();

	void SetCursor(cGenericObject* c);
	void GetCursorPosition(cVector3d&);
	void UpdateCursor(dmat4x4);
	
	virtual void SetFlag(ModeFlag*);
	virtual void ModeChange();

};